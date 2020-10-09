# tempomap
Maps an image sequence to a canvas using the pixel values of a grayscale map as time reference.

#### Usage
`./tempomap [map] [image sequence] [iterations] [rgb/grayscale] [out]`<br/>
(rgb = 1, grayscale = 0)

#### Examples
[Jellyfish Test File](http://www.jell.yfish.us/)
converted to bitmap image sequence with ffmpeg:<br/>
`ffmpeg -i jellyfish.mkv -vframes 256 -start_number 0 imgs/%03d.bmp`

`./tempomap imgs/000.bmp imgs 4 1 examples/out_rgb.bmp` (four iterations in rgb-mode)<br/>
![exmaple 1](examples/out_rgb.bmp)
`./tempomap imgs/000.bmp imgs 1 0 examples/out_gray.bmp` (one iteration in grayscale-mode)<br/>
![exmaple 2](examples/out_gray.bmp)

#### Assumptions
three channel bitmap images (RGB)<br/>
256 images in image sequence

#### Build
On Linux:<br/>
For Linux: `make`<br/>
For Windows `make windows`

#### Releases
Static binaries are available for Linux and Windows at [Releases](https://github.com/plensch/tempomap/releases/tag/v0.1r)

#### Dependencies
stb_image.h and stb_image_write.h from the awesome [stb libraries](https://github.com/nothings/stb) (included in repo)
