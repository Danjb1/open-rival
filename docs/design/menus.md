# Menu System

## Aims

- We should be able to create a menu, and then say `menuRenderer.render(menuRoot)` to render it
- We should be able to pass input to a menu and receive callbacks based on the component that was clicked
- MenuElements should be positioned using layouts, not absolute positions
- MenuElements should exist in a recursive hierarchy
    - A layout is itself a MenuElement, and can contain other MenuElements
- Each MenuElement should contain their own Renderables (akin to SpriteComponents)
    - These should only be updated on the GPU when a component gets marked as dirty
    - These should use the depth buffer so that dropdowns can be rendered on top of other elements
    - These should be positioned independently of the MenuElement's hitbox
- Input gets passed to the MenuElement root, and trickles down
    - Each MenuElement whose hitbox is under the mouse returns a bool to say whether it consumed the input
    - MenuElements should be traversed in a defined order (dropdowns should be added at position 0 as they are on top)
- We should be able to debug draw hitboxes

## MenuElements

- Basic layouts (HorizontalLayout, VerticalLayout, GridLayout, ScrollableList)
- Label
- PopUp
- Inset
- Button
- Dropdown (when clicked, adds a child element)
- MenuImage
- Background

## Example (In-game HUD)

- HorizontalLayout (anchored to the bottom of the screen somehow)
    - MiniMap (bespoke MenuElement)
    - MenuImage (main panel background)
        - HorizontalLayout
            - VerticalLayout
                - GridLayout
                    - ActionButton (bespoke MenuElement)
                    - ActionButton
                    - etc.
                - HorizontalLayout
                    - Inventory / Building menu (added / removed dynamically)
            - Unit portrait, health bar, etc.
    - MenuImage (stats panel background)
        - VerticalLayout
            - Label (Damage: X)
            - Label (Speed: X)
            - etc.
