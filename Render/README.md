
# Render

## Table of contents

1. [Shed](#shed)
2. [Axe](#axe)
3. [Workbench](#workbench)
4. [Toolbox](#toolbox)
5. [Coffee Cup](#coffee-cup)
6. [Ground](#ground)
7. [Scene](#scene)

<br>

## Shed

The shed is the main model in my scene and, to create a gloomy feel, I wanted to have the majority of the scene's light emanate from within it.  To do this, the model features many gaps and holes to cast long shadows and shine volumetric rays of through in the atmosphere around it.

![Shed Modelling](/Appendices/images/ShedModelling.png)

The shed model is comprised of multiple, modular components.  The wooden frames were drawn in 2D and extruded to form the basis for the structure.  The lower walls were then created with gaps between the planks to cast shadows on the ground.  The upper wall is made of hatched wood and features holes to let the light shine through.

Making the frames and the wall components modular allowed me to quickly clone different parts and form all 4 walls.  To finish the model, I created a slanted wooden roof and placed it on top.

![Shed Materials](/Appendices/images/ShedMaterials.png)

To use high-resolution material textures without exceeding the storage limit, I used different augmentations of the same texture for multiple components of this model.  I found a free-to-use plank PBR texture on [textures.com](https://www.textures.com) with colour, normal, and roughness maps.  Each material uses the same normal map, but they alter the exposure value of the colour map before use; this way, one image file can be used for all of the materials.  I also decreased the exposure of the roof's roughness map, to exaggerate the specular highlight of the moonlight in certain angles.

The frames and hatched wooden walls initially had very stretched UV maps.  I fixed this issue by repositioning the texture coordinates and allowing the texture to repeat.  Because the texture used is seamless, the pattern repetition isn't visible.

<br>

## Axe

Starting with a cube, I used Maya's deform tools to form a natural-looking axe head.  I used the flare deformer to pinch the axe head to a sharp point, and the squash tool allowed me to achieve the curves along the face of the axe.  I then used the bevel tool to round off the edges behind the handle intersection.

![Axe Design](/Appendices/images/AxeDesign.png)

To shape the handle, I used the bend tool.  The bends are deliberately asymmetric to create a more natural-looking wooden handle.

For the axe head's material, I used a rusted iron PBR texture with colour and normal map.  The tip of the blade uses a different iron texture to show wear on the blade.  An oak PBR texture with colour and normal map was used to create the handle.  Once again, the UVs had to be adjusted and the texture repeated to achieve a good result.

<br>

## Workbench

![Workbench Design A](/Appendices/images/WorkbenchA.png)

To have an animated component to the scene, I wanted to have a workbench with moving vice.  The tabletop is made from the same oak texture as the axe's handle, only with a different colour exposure.  I created the tabletop, vice body, and legs from cubes, scaled and stretched in different directions.

To add detail to the bench, I added small iron bolts to the front of the table, and where the legs join to the tabletop.  These are created with colour, normal, and roughness maps to create a realistic shine under the light.  I also created a handle for the vice with the same material.  The vice's handle has its centroid as the model centre so that rotations can be done easily to animate it turning.

![Workbench Design B](/Appendices/images/WorkbenchB.png)

The table body, the vice, and the vice's handle are all separate objects that can be moved independently.

<br>

## Toolbox

To add some intricacy to the scene, I designed a box of assorted tools.

![Toolbox Design A](/Appendices/images/ToolboxA.png)

The main body was quite simple to model.  A cube was subdivided on two faces and extruded inward to create space for the tools inside.  To texture the model,  I used two materials: a brushed steel PBR for the inside and a red paint PBR for the outside, each with colour and normal maps.  Though other models had multiple textures per mesh, this model has multiple textures per vertex.  This was a problem I had to solve during the OpenGL import, and is covered in the [interaction section](../Interaction/README.md).

![Toolbox Design B](/Appendices/images/ToolboxB.png)

I created the toolbox lid by taking Maya's boolean difference between smaller and larger semi-cylinders.  The handles were created from a long cylinder, bent in a similar way to the axe handle.  The drawer was created similarly to the toolbox's body and the handle cloned from the lid.

![Toolbox Design C](/Appendices/images/ToolboxC.png)

Finally, a small wrench was made by using the 2D cutting tool, extruding the drawing, and beveling the edges.  This wrench was cloned and different sizes added to the toolbox.  The wrenches use the same PBR texture as the workbench's bolts.  A screwdriver was modeled by deforming a cylinder with the squash tool and flaring the end to create a Philips head.

<br>

## Coffee Cup

The coffee mug started as a cylinder, flaring the bottom inward slightly and using the squash deformation to curve the mug outward.  Using the boolean difference between the mug itself and a slightly smaller clone, I created the hollow centre.

The handle was then created by bending a long cylinder with two separate bend deformations.  A 3D slice was also used to make sure it sat flush with the curved mug.

![Coffee Cup Design](/Appendices/images/CoffeeCupDesign.png)

I experimented with a few textures for the coffee mug for a while, but found no nice textures for the aesthetic I wanted.  I decided to colour it flat white but, to add some detail, used the normal map from the iron material.  This created a clean look to the mug with small imperfections under the light.

Finally, I added coffee to the mug by taking a flat circle and deforming it with Maya's wave deformation.  In the future, I would explore using a flat plane in the model and dynamically rippling it with sine-wave deformation in the shader.  The coffee uses a constant brown colour with a low constant roughness value to reflect the light.

<br>

## Ground

![Ground Design](/Appendices/images/GroundDesign.png)

I found a high-detail ground PBR texture that suited the scene.  It used colour, normal, roughness and displacement maps to provide intricate details.  I used the sculpt tool to add natural-looking bumps the the ground model, and applied the texture.  Using a displacement map allowed the small details such as tree roots to be tessellated, raised, and cast shadows on other parts of the ground.

One issue with the texture used for the ground was that, even though it was seamless, it had obvious repeating patterns.  This meant that it couldn't be repeated too many times in order to hide the pattern.  Something that could be done with more time is to interpolate between different offsets of the texture based on a noise function.  This would hopefully help to disguise some of the repeating pattern.

<br>

## Scene

This is the final render, made in Arnold:

![Scene Render](/Render/Render.png)

To enhance the gloomy atmosphere of the scene, I created a dark skybox and added low, silvery moonlight as a directional Arnold light.  I created an Arnold photometric light inside the shed that was little warmer in colour.  Finally, I added some atmospheric fog to the scene.  This created faint light beams in the atmosphere around the scene, and diffused some of the light inside the shed.

![Scene Design](/Appendices/images/SceneDesign.png)

As designed, the gaps in the lower walls cast long, soft shadows on the ground around the shed.  The door of the shed also allows light to flood out and draw attention to the entrance.

Inside the shed, the high light levels diffuse in the fog and bounce around the interior.

As can be seen, especially on the ground model, the use of PBR textures significantly enhances the way the objects interacts with the light.
