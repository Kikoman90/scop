# SCOP

Scop is a projet related to computer graphics, and part of my formation at school 42.
It is a good introduction to OpenGL and 3D graphics more generally, as it requires a solid understanding of several notions such as draw calls, openGL buffer and array objects, and vector maths. We also had to write our own "math" library, in which I implemented some useful methods to manipulate vectors, matrices, and quaternions.<br />
As a bonus, I added multisample anti-aliasing and object picking by using renderbuffers, basic lighting/shading, and object manipulation via 3D handles (translation, rotation, scaling).

## preview

<img align="center"
src="https://github.com/Kikoman90/scop/blob/master/screenshots/scop_1.png" width="100%" height="544px" />
<br />
<br />
<img align="center"
src="https://github.com/Kikoman90/scop/blob/master/screenshots/scop_2.png" width="100%" height="544px" />

## install and launch

```bash
git clone https://github.com/Kikoman90/scop ~/scop
cd ~/scop && make
```
Launch the program with one or more parameters: the name of the <i>.obj</i> files you would like to open.
The format of these files is <i>wavefront obj</i>. These files define a 3D model composed of vertices, linked together to create faces. They can also contain vertex normals and texture coordinates. Also, those files can reference one or multiple <i>.mtl</i> files, the latter which define material properties for shading.

Example : ⇣
```bash
./scop models/42_uv.obj
```

## input

<table width="100%">
<thead>
<tr>
<td width="55%" height="60px" align="center" cellpadding="0">
<strong>Description</strong>
</td>
<td width="40%" align="center" cellpadding="0">
<span style="width:70px">&nbsp;</span><strong>Key(s)</strong><span style="width:50px">&nbsp;</span>
</td>
</tr>
</thead>
<tbody>
<tr>
<td valign="top" height="30px">exit program</td>
<td valign="top" align="center"><kbd>&nbsp;esc&nbsp;</kbd></td>
</tr>
<tr>
<td valign="top" height="30px">orbit camera</td>
<td valign="top" align="center"><kbd>&nbsp;right-click&nbsp;</kbd></td>
</tr>
<tr>
<td valign="top" height="30px">pan camera</td>
<td valign="top" align="center"><kbd>&nbsp;shift&nbsp;</kbd><span>&nbsp;+&nbsp;</span><kbd>&nbsp;right-click&nbsp;</kbd></td>
</tr>
<tr>
<td valign="top" height="30px">zoom camera</td>
<td valign="top" align="center"><kbd>&nbsp;ctrl&nbsp;</kbd><span>&nbsp;+&nbsp;</span><kbd>&nbsp;right-click&nbsp;</kbd></td>
</tr>
<tr>
<td valign="top" height="30px">select object (hold <kbd>&nbsp;shift&nbsp;</kbd> for multi-selection)</td>
<td valign="top" align="center"><kbd>&nbsp;left-click&nbsp;</kbd></td>
</tr>
<tr>
<td valign="top" height="30px">translation handle</td>
<td valign="top" align="center"><kbd>&nbsp;W&nbsp;</kbd></td>
</tr>
<tr>
<td valign="top" height="30px">rotation handle</td>
<td valign="top" align="center"><kbd>&nbsp;E&nbsp;</kbd></td>
</tr>
<tr>
<td valign="top" height="30px">scale handle</td>
<td valign="top" align="center"><kbd>&nbsp;R&nbsp;</kbd></td>
</tr>
<tr>
<td valign="top" height="30px">activate/deactivate auto-rotate</td>
<td valign="top" align="center"><kbd>&nbsp;Z&nbsp;</kbd></td>
</tr>
<tr>
<td valign="top" height="30px">switch handles space (world/local)</td>
<td valign="top" align="center"><kbd>&nbsp;X&nbsp;</kbd></td>
</tr>
<tr>
<td valign="top" height="30px">switch face gradient (grayscale/RGB)</td>
<td valign="top" align="center"><kbd>&nbsp;C&nbsp;</kbd></td>
</tr>
<tr>
<td valign="top" height="30px">smooth shader transition</td>
<td valign="top" align="center"><kbd>&nbsp;space&nbsp;</kbd></td>
</tr>
<tr>
<td valign="top" height="30px">switch skybox</td>
<td valign="top" align="center"><kbd>&nbsp;S&nbsp;</kbd></td>
</tr>
<tr>
<td valign="top" height="30px">switch model texture</td>
<td valign="top" align="center"><kbd>&nbsp;T&nbsp;</kbd></td>
</tr>
</tbody>
</table>

## Contact & contribute

If you want to contact me, or help improve this project, send me a mail at **fsidler@student.42.fr**
