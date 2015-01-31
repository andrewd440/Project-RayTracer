RayTracer
=========
This was a side project that I used to further improve my knowledge on the subject of computer graphics and ray tracing. Some rendering features of this c++ ray-tracer include:
<ul>
	<li>Light Reflection and Refraction</li>
	<li>Transparent Surfaces</li>
	<li>Diffuse Texture-Mapping (.tga image format)</li>
	<li>User Controlled Soft Shadows</li>
	<li>User Controlled Super-Sampling Anti-Aliasing</li>
	<li>.Obj Triangle Mesh Model Loading</li>
</ul>
Intersection acceleration structures include the use of a KD-Tree for scene partitioning and hierarchical bounding volumes are used for mesh models.

Image variables are controlled through a text file in the main directory. Here, users can control output resolution, the number of shadow samples taken, super-sampling level, the output image name and a path to the scene config file. The scene config file is a custom .scn extension text file that contains details about the objects in the scene.

Example of including a .obj mesh model, a cube, and sphere in a scene file.
<a href="https://andrewdlowry.files.wordpress.com/2015/01/sceneconfig.png"><img class="wp-image-54 size-large" src="https://andrewdlowry.files.wordpress.com/2015/01/sceneconfig.png?w=788" alt="Scene File" width="788" height="327" /></a>

&nbsp;

Render Images:

<a href="https://andrewdlowry.files.wordpress.com/2015/01/boxpeak.png"><img class="aligncenter wp-image-55 size-large" src="https://andrewdlowry.files.wordpress.com/2015/01/boxpeak.png?w=788" alt="BoxPeak" width="788" height="433" /></a>

<a href="https://andrewdlowry.files.wordpress.com/2015/01/supersamplingandsoftshadows.png"><img class="aligncenter wp-image-57 size-large" src="https://andrewdlowry.files.wordpress.com/2015/01/supersamplingandsoftshadows.png?w=788" alt="SuperSamplingAndSoftShadows" width="788" height="413" /></a>

<a href="https://andrewdlowry.files.wordpress.com/2015/01/super3x.png"><img class="alignnone wp-image-56 size-large" src="https://andrewdlowry.files.wordpress.com/2015/01/super3x.png?w=788" alt="Super3x" width="788" height="443" /></a>
