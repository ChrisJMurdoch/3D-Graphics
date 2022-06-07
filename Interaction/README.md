
# Interactive demo

## Table of contents

1. [Importing OBJ](#importing-the-.obj-file)
1. [Importing MTL](#importing-the-.mtl-file)
2. [Materials](#materials)
2. [Lighting](#lighting)
3. [Animation and Interaction](#animation-and-interaction)
4. [Points of Interest](#points-of-interest)

<br>

## Importing the .obj file

To start with, I exported the entire Maya scene as a single obj file.  Tweaking the shader slightly to get crude directional lighting, I ended up with this:

![Diffuse Shader](/Appendices/images/DiffuseShader.png)

This part was easy enough as the template already had obj parsing.  All that had to be done was to add functionality to render multiple models.

## Importing the .mtl file

As described earlier, my scene uses multiple textures per model, and some use multiple textures per vertex.  To texture these correctly, I designed custom functionality to parse the mtl file.

When parsing the indices, the obj file will occasionally assign a new material to the following indices with the command 'usemtl material_name'.  When the obj parser sees this, it maps that material name to an int, and sets the active material int to that value; future indices are then set to have that int as their texture z coordinate.

With all the indices mapped to specific ints, each representing a different material, the system needs an easy to map those ints to texture files.  Keeping the string->int map used before, the mtl file is read and the filenames for all the textures are retrieved.  A texture array is then generated, and stbi_image used to import the images into the array.

When rendering these textures in the fragment vertex, the texture's z coordinate is stored as a vertex attribute and the uniform 2DSampler can be used to retrieve the correct texture sample.

![OpenGL Shed](/Appendices/images/OpenGLShed.png)

The objects correctly map to their textures but, like UE4, some data from Maya is lost.  Constant colours used for the coffee model aren't moved over, so I decided to create a flat colour texture for them to sample.  Also, the ground's texture doesn't store the repetition.  I solved this by scaling the texture, but it comes at the cost of resolution.  Because array textures must have textures of the same size, scaling up the ground texture would mean I have to scale up all of the textures, which would have used too much storage.

![Vertex Issue](/Appendices/images/VertexIssue.png)

One other issue that arose when importing the model was that some vertices connected strangely.  I found out that this was because the faces were being imported as polygons instead of triangles, but it was easily rectified with Maya's triangulate tool.  It would be nice to have a feature in my interactive application to automatically triangulate polygons, but it proved to be too large a task for the given time frame.

<br>

## Materials

Because the application automatically parses the mtl file, the materials used are the same as in Maya and UE4.  PBR functionality such as normal and roughness maps haven't been implemented, but global attributes such as roughness and reflectiveness are configurable as floating-point constants in the application.

<br>

## Lighting

The application uses the Phong lighting model, with configurable lighting weights.  Ambient, diffuse and specular are all calculated per light source and combined into one output.

![Specular](/Appendices/images/Specular.png)

The application also features the ability to set light colours and brightness:

![Coloured Lights](/Appendices/images/ColouredLights.png)

To emulate real-world lighting, I also implemented light decay.  This lets me put a warm light inside the shed, without it lighting up the entire scene:

![Light Decay](/Appendices/images/LightDecay.png)

I experimented with exponential light decay, but the parameters proved quite fiddly and linear was easier to control.

<br>

## Animation and Interaction

To animate the objects in an easy-to-use way, I created a function in the model instance's class:

![Animate Function](/Appendices/images/AnimateFunction.png)

This function simply takes in a start position, an end position, and an interpolant between 0 and 1.  When the interpolant is at 0, the model's position will be a.  Increasing the interpolant to 1 moves the object toward position b.  By using this simple function, I can easily define two points and have the object move between them in a loop, using an interpolant such as sin(glfwGetTime()).

![Vice Animation](/Appendices/images/ViceAnimation.png)

The scene is deliberately dark; my original vision was to have a scene almost pitch black outside the warm light pouring out from within the shed.  However, this means some of the details in the scene - especially the axe - can't easily be seen.  To solve this issue without changing the feel of the scene, I've added a portable flashlight.  Pressing the F key at any time will toggle a light that follows the player.  Not just to see some details in the dark, it helps highlight the specular reflection in some of the curved models.

![Flashlight](/Appendices/images/Flashlight.png)

## Points of interest

As there's quite a lot of code in the repository, I have compiled a list of the most important/interesting areas:

1. **shaders/**: Vertex and Fragment shaders.  This if where all of the texture-sampling and lighting code is written.
2. **instance.cpp**: Class written to handle movement, animation and rendering of individual model instances.
3. **Mesh.cpp > materialTextureFiles() & initTextures()**: Material-mapping and texture-initialising code.
4. **OBJLoader.cpp > setMaterial() & writeMaterial()**: MTL-parsing code.
5. **Source.cpp > update() & render()**: General interaction and rendering code.
