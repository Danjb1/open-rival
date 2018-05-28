package main;

import java.io.EOFException;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.ArrayList;
import java.util.List;

public class ImageExtractor implements AutoCloseable {

    ////////////////////////////////////////////////////////////////////////////
    // Main
    ////////////////////////////////////////////////////////////////////////////
    
    public static void main(String[] args) {
        try (ImageExtractor ie = new ImageExtractor("D:/rivalrealms/DATA")) {
            ie.extract();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // ImageGroup
    ////////////////////////////////////////////////////////////////////////////
    
    private static class ImageGroup {
        
        private int unknown;
        private int imageWidth;
        private int imageHeight;
        private List<ImageSubGroup> subGroups;
        
        public ImageGroup(int numSubGroups, int unknown, int imageWidth, int imageHeight) {
            this.unknown = unknown;
            this.imageWidth = imageWidth;
            this.imageHeight = imageHeight;
            
            subGroups = new ArrayList<>(numSubGroups);
        }
        
        @Override
        public String toString() {
            return String.format("ImageGroup (%d x %d) - %d - with %d subgroups",
                    imageWidth, imageHeight, unknown, subGroups.size());
        }
        
    }

    ////////////////////////////////////////////////////////////////////////////
    // ImageSubGroup
    ////////////////////////////////////////////////////////////////////////////
    
    private static class ImageSubGroup {
        
        private List<ImageMetadata> primaryImages;
        private List<ImageMetadata> secondaryImages;
        
        public ImageSubGroup(int numPrimaryImages, int numSecondaryImages) {
            primaryImages = new ArrayList<>(numPrimaryImages);
            secondaryImages = new ArrayList<>(numSecondaryImages);
        }
        
        @Override
        public String toString() {
            return String.format("ImageSubGroup: %d + %d images",
                    primaryImages.size(), secondaryImages.size());
        }
        
    }

    ////////////////////////////////////////////////////////////////////////////
    // ImageMetadata
    ////////////////////////////////////////////////////////////////////////////
    
    private static class ImageMetadata {
        
        private int fileOffset;
        private int fileLength;
        private int section;
        private int unknown1;
        private int unknown2;

        public ImageMetadata(
                int fileOffset,
                int fileLength,
                int section,
                int unknown2,
                int unknown3) {
            this.fileOffset = fileOffset;
            this.fileLength = fileLength;
            this.section = section;
            this.unknown1 = unknown2;
            this.unknown2 = unknown3;
        }
        
        @Override
        public String toString() {
            return String.format("ImageMetadata (%d, %d) - %d, %d, %d",
                    fileOffset, fileLength, section, unknown1, unknown2);
        }
        
        public long getOffset() {
            if (section == 1) {
                return FINAL_SECTION_OFFSET + fileOffset; 
            }
            return fileOffset;
        }
        
    }

    ////////////////////////////////////////////////////////////////////////////
    // ImageExtractor
    ////////////////////////////////////////////////////////////////////////////
    
    // Value found by trial and error; any higher and we hit EOF
    private static final int NUM_GROUPS = 188;

    public static final int MAIN_SECTION_OFFSET = 309700;
    public static final int FINAL_SECTION_OFFSET = 16777744;
    
    private RandomAccessFile imagesReader;
    private RandomAccessFile imOffsReader;
    
    private List<ImageGroup> imageGroups = new ArrayList<>();
    
    public ImageExtractor(String dir) {
        
        File imagesFile = new File(dir + "/IMAGES.DAT");
        File imOffsFile = new File(dir + "/IMOFFS.DAT");
        
        try {
            imagesReader = new RandomAccessFile(imagesFile, "r");
            imOffsReader = new RandomAccessFile(imOffsFile, "r");
        } catch (FileNotFoundException ex) {
            ex.printStackTrace();
        }
    }

    private void extract() throws IOException {

        long numImages = readUnsignedInt(imagesReader, true);
        System.out.println("Number of images: " + numImages);
        
        readImageOffsets();

        System.out.println(String.format("Read %d / %d bytes",
                imOffsReader.getFilePointer(), imOffsReader.length()));
        
        for (ImageGroup group : imageGroups) {
//            System.out.println(group);
            
            for (ImageSubGroup subGroup : group.subGroups) {
//                System.out.println("    " + subGroup);
                
                for (ImageMetadata metadata : subGroup.primaryImages) {
//                    System.out.println("        " + metadata);
                    readImage(metadata, group.imageWidth, group.imageHeight);
                }

                for (ImageMetadata metadata : subGroup.secondaryImages) {
//                    System.out.println("        " + metadata);
                    readImage(metadata, group.imageWidth, group.imageHeight);
                }
            }
        }
    }

    private void readImageOffsets() throws IOException {

        for (int i = 0; i < NUM_GROUPS; i++) {
            imOffsReader.seek(i * 4);
            long offset = readUnsignedInt(imOffsReader, true);
            readImageGroup(offset);
        }
    }

    private void readImageGroup(long offset) throws IOException {
        
        imOffsReader.seek(offset);
        
        int numSubGroups = readUnsignedByte(imOffsReader);
        int unknown      = readUnsignedByte(imOffsReader);
        int imageWidth   = readUnsignedByte(imOffsReader);
        int imageHeight  = readUnsignedByte(imOffsReader);
        
        ImageGroup group = new ImageGroup(
                numSubGroups, unknown, imageWidth, imageHeight);
        imageGroups.add(group);
        
        for (int i = 0; i < numSubGroups; i++) {

            int numPrimaryImages   = readUnsignedByte(imOffsReader);
            int numSecondaryImages = readUnsignedByte(imOffsReader);
            
            ImageSubGroup subGroup = new ImageSubGroup(
                    numPrimaryImages, numSecondaryImages);
            group.subGroups.add(subGroup);
            
            readImageMetadata(numPrimaryImages, subGroup.primaryImages);
            readImageMetadata(numSecondaryImages, subGroup.secondaryImages);
        }
    }

    private void readImageMetadata(int numImages, 
            List<ImageMetadata> metadata) throws IOException {

        for (int i = 0; i < numImages; i++) {

            // 4 bytes
            int fileOffset = readUnsigned3Bytes(imOffsReader, true);
            int section    = readUnsignedByte(imOffsReader);

            // 4 bytes
            int fileLength = readUnsignedShort(imOffsReader, true);
            int unknown1   = readUnsignedByte(imOffsReader);
            int unknown2   = readUnsignedByte(imOffsReader);
            
            metadata.add(new ImageMetadata(
                    fileOffset, fileLength, section, unknown1, unknown2));
        }
        
    }

    private void readImage(ImageMetadata metadata, int width, int height)
            throws IOException {
        
        long offset = metadata.getOffset();
        imagesReader.seek(offset);
        
        try {
            int initialByte = readUnsignedByte(imagesReader);
            System.out.println("Image at " + offset + " starts with byte: " +
                    initialByte);
        } catch (EOFException ex) {
            System.out.println("Offset " + offset + " is too high!");
        }
    }

    @Override
    public void close() throws Exception {
        
        if (imagesReader != null) {
            imagesReader.close();
        }
        
        if (imOffsReader != null) {
            imOffsReader.close();
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // Utility Methods
    ////////////////////////////////////////////////////////////////////////////
    
    private static long readUnsignedInt(RandomAccessFile file,
            boolean littleEndian) throws IOException {
        
        int value = file.readInt();
        
        if (littleEndian) {
            value = Integer.reverseBytes(value);
        }
        
        return Integer.toUnsignedLong(value);
    }

    private static int readUnsignedShort(RandomAccessFile file,
            boolean littleEndian) throws IOException {

        short value = file.readShort();
        
        if (littleEndian) {
            value = Short.reverseBytes(value);
        }
        
        return Short.toUnsignedInt(value);
    }

    private static int readUnsigned3Bytes(RandomAccessFile file,
            boolean littleEndian) throws IOException {

        if (littleEndian) {
            return file.read() |
                    file.read() << 8 |
                    file.read() << 16;            
        }

        return file.read() << 16 |
                file.read() << 8 |
                file.read();     
    }

    private static int readUnsignedByte(RandomAccessFile file)
            throws IOException {
        return file.read();
    }
    
}
