
# Animation

## Table of contents

1. [Importing](#importing)
2. [Materials & Lighting](#materials-&-lighting)
3. [Animation](#animation)

<br>

## Importing

To move the models from Maya to Unreal, I exported them as Filmbox (fbx) files.  This was mostly a smooth transition but there were a few small issues to work out.

### Texture file types

One issue was that, unlike Maya, UE4 does not seem to natively support .tx and .tiff files.  I looked into using extensions to get around the problem but, in the end, I decided that it was better long-term to convert the files to png format.  I converted the colour, normal, roughness and displacement maps with Paint 3D and, because they all use standard channels, the conversion worked.  I imported them back into UE4, replaced the material references, and everything worked.

### Flat colours

Another issue that arose from importing the model was that the flat-coloured mug and coffee were no longer coloured.  One option was to use a flat-coloured texture png for the material, but it would have been a waste of storage.  Instead, I used UE4's node editor to link the colouring of the materials to constant vectors.  This saved on storage and allowed me to easily tweak the colouring of the materials in-engine.

![Flat colours](/Appendices/images/MaterialColouring.png)

### Texture exposure

A feature provided in Maya but not UE4 is to alter the exposure of a texture.  I used this feature to have multiple materials made from different shades of the same png texture file.  To emulate this feature in UE4, I multiplied the sample output by a constant value.  This allowed me to continue using multiple materials with one shared texture file.

### Model centres

For convenience, most of the separate models were exported from Maya with their centres as the scene centre.  This meant that placing them all at (0,0,0) resulted in them all being in the correct place.  One issue with this, however, was that the rotations required for animation had the wrong axis.  I went back to Maya and exported them all again, with their centroids as the centre.  There were also ways to do this in UE4 but I found this method simpler.

<br>

## Materials & Lighting

### Metallic

To achieve a more realistic-looking metal texture for the tools, I used UE4's 'metallic' node.  I linked constant values to the metallic and roughness nodes, and a texture sample to the colour node.  To save on storage, I have omitted maps such as normal and AO where they didn't contribute much to the output.

![Iron Material](/Appendices/images/IronMaterial.png)

### Rotation

I used a custom rotator to augment the hatched wood texture's UVs.  This rotated the input textures and meant I didn't have to use a separate ones.

![Hatched Material](/Appendices/images/HatchedMaterial.png)

### Ambient Occlusion

UE4 handles displacement maps a little differently to Maya so the lighting on the ground looked a little flat.  To fix this, I downloaded the ambient occlusion map for the texture.  This defines the parts of the texture where light struggles to reach.  Using the AO map, the ground looks a lot more realistic.

![Ground Material](/Appendices/images/GroundMaterial.png)

### Experimenting with UE4 nodes

I wanted to add a scratched effect to the red paint on the toolbox but I didn't have a roughness map to reflect this.  Instead, I used the roughness map from a rusty metal texture.  The original output was too shiny but, after adding a couple of math nodes, I was able to alter the effect it had.

![Paint Material](/Appendices/images/PaintMaterial.png)

<br>

## Animation

The final [animation](Video.avi) uses 5 actors and multiple camera and atmospheric effects.

![Animation Frames](/Appendices/images/AnimationFrames.png)

The animation uses a CineCameraActor with keyframes in different positions to smoothly traverse the scene.  As the camera enters the shed, the FOV opens up to show more of the scene inside.

![Workbench Animation](/Appendices/images/WorkbenchAnimation.png)

The workbench consists of three parts:  the bench, the vice, and the vice handle.  The vice and its handle move in and out, and the handle rotates as it moves.  This movement is shown in the animation.

![Toolbox Animation](/Appendices/images/ToolboxAnimation.png)

The components of the toolbox all have their centre at the back of the lid.  This is so that the can all move together, and the lid can rotate as if it were joined to the toolbox with a hinge.  During the animation, the toolbox opens to reveal the tools inside.
