#include "gfx/opengl/renderable/AtlasRenderable.h"

#include <algorithm>  // std::min

#include "gfx/opengl/GLUtils.h"
#include "gfx/opengl/Shaders.h"
#include "utils/LogUtils.h"

namespace Rival {

AtlasRenderable::AtlasRenderable(std::shared_ptr<const TextureAtlas> texAtlas, int maxSprites)
    : texAtlas(texAtlas)
    , maxSprites(maxSprites)
{
    // Generate VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate VBOs
    glGenBuffers(1, &positionVbo);
    glGenBuffers(1, &texCoordVbo);
    glGenBuffers(1, &ibo);

    // Determine the primitive that should be drawn
    drawMode = (maxSprites == 1) ? GL_TRIANGLE_FAN : GL_TRIANGLES;

    // Determine the number of indices per sprite
    indicesPerSprite =
            (drawMode == GL_TRIANGLE_FAN) ? GLUtils::numIndicesForTriangleFan : GLUtils::numIndicesForTriangles;

    // Initialize position buffer with empty data
    glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
    glVertexAttribPointer(
            Shaders::vertexAttribIndex,
            numVertexDimensions,
            GL_FLOAT,
            GL_FALSE,
            numVertexDimensions * sizeof(GLfloat),
            nullptr);
    int positionBufferSize = maxSprites * numVertexDimensions * indicesPerSprite * sizeof(GLfloat);
    glBufferData(GL_ARRAY_BUFFER, positionBufferSize, NULL, GL_DYNAMIC_DRAW);

    // Initialize tex co-ord buffer with empty data
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
    int texCoordBufferSize = maxSprites * numTexCoordDimensions * indicesPerSprite * sizeof(GLfloat);
    glVertexAttribPointer(
            Shaders::texCoordAttribIndex,
            numTexCoordDimensions,
            GL_FLOAT,
            GL_FALSE,
            numTexCoordDimensions * sizeof(GLfloat),
            nullptr);
    glBufferData(GL_ARRAY_BUFFER, texCoordBufferSize, NULL, GL_DYNAMIC_DRAW);

    // Initialize index buffer - this should never need to change
    std::vector<GLuint> indexData;
    indexData.reserve(maxSprites * indicesPerSprite);
    for (int i = 0; i < maxSprites; i++)
    {

        unsigned int startIndex = i * numVerticesPerSprite;

        if (drawMode == GL_TRIANGLE_FAN)
        {
            indexData.push_back(startIndex);
            indexData.push_back(startIndex + 1);
            indexData.push_back(startIndex + 2);
            indexData.push_back(startIndex + 3);
        }
        else if (drawMode == GL_TRIANGLES)
        {
            indexData.push_back(startIndex);
            indexData.push_back(startIndex + 1);
            indexData.push_back(startIndex + 2);
            indexData.push_back(startIndex + 2);
            indexData.push_back(startIndex + 3);
            indexData.push_back(startIndex);
        }
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER, maxSprites * indicesPerSprite * sizeof(GLuint), indexData.data(), GL_STATIC_DRAW);

    // Enable vertex attributes
    glEnableVertexAttribArray(Shaders::vertexAttribIndex);
    glEnableVertexAttribArray(Shaders::texCoordAttribIndex);
}

AtlasRenderable::AtlasRenderable(AtlasRenderable&& other) noexcept
    : vao(other.vao)
    , positionVbo(other.positionVbo)
    , texCoordVbo(other.texCoordVbo)
    , ibo(other.ibo)
    , drawMode(other.drawMode)
    , indicesPerSprite(other.indicesPerSprite)
    , maxSprites(other.maxSprites)
    , numSprites(other.numSprites)
    , texAtlas(other.texAtlas)
{
    // Reset the source object so its destructor is harmless
    other.vao = 0;
}

AtlasRenderable::~AtlasRenderable()
{
    if (!vao)
    {
        return;
    }

    // Clean up
    glBindVertexArray(vao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
        glDeleteBuffers(1, &positionVbo);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
        glDeleteBuffers(1, &texCoordVbo);
        glBindBuffer(GL_ARRAY_BUFFER, ibo);
        glDeleteBuffers(1, &ibo);
    }
    glDeleteVertexArrays(1, &vao);
    glBindVertexArray(0);
}

void AtlasRenderable::setImages(const std::vector<AtlasImage>& images)
{
    if (static_cast<int>(images.size()) > maxSprites)
    {
        LOG_WARN("Tried to render {} images, but the maximum is {}", images.size(), maxSprites);
    }
    numSprites = std::min(static_cast<int>(images.size()), maxSprites);

    std::vector<GLfloat> vertexData;
    std::vector<GLfloat> texCoordData;
    vertexData.reserve(numSprites * numVerticesPerSprite * numVertexDimensions);
    texCoordData.reserve(numSprites * numVerticesPerSprite * numTexCoordDimensions);

    for (int i = 0; i < numSprites; ++i)
    {
        const AtlasImage& image = images[i];
        const float x1 = image.position.x;
        const float y1 = image.position.y;
        const float x2 = x1 + image.size.x;
        const float y2 = y1 + image.size.y;
        const float z = 0;

        /* clang-format off */
        vertexData.insert(vertexData.end(), {
            x1, y1, z,
            x2, y1, z,
            x2, y2, z,
            x1, y2, z
        });
        /* clang-format on */

        const std::vector<float> texCoords = texAtlas->getTexCoords(image.imageKey);
        texCoordData.insert(texCoordData.end(), texCoords.cbegin(), texCoords.cend());
    }

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, positionVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexData.size() * sizeof(GLfloat), vertexData.data());

    glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, texCoordData.size() * sizeof(GLfloat), texCoordData.data());
}

void AtlasRenderable::draw() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, getTextureId());

    glBindVertexArray(vao);
    glDrawElements(drawMode, indicesPerSprite * numSprites, GL_UNSIGNED_INT, nullptr);
}

}  // namespace Rival
