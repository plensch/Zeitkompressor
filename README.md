<p>
  <img src="icon.svg" alt="Icon" width="80"/>
  <h1>Zeitkompressor</h1>
</p>

Maps an image sequence to a canvas using the pixel values of a grayscale map as time reference.

I seems that this idea has been around for some time:<br>
[t-morph - Roman Haefeli](http://www.romanhaefeli.net/projekte/out_of_frame/index.php)<br>
[tx-transform - Martin Reinhart ](http://www.tx-transform.com/Eng/index.html)<br>

#### Usage
`./zeitkompressor [map] [image sequence] [iterations] [rgb/grayscale] [out]`<br>
(rgb = 1, grayscale = 0)<br>
(sequence = 1, static = 0)<br>
When sequence = 1 then [out] should be a directory<br>

#### Examples
[Jellyfish Test File](http://www.jell.yfish.us/)
converted to bitmap image sequence with ffmpeg:<br>
`ffmpeg -i jellyfish.mkv -vframes 256 -start_number 0 imgs/%03d.bmp`

`./zeitkompressor imgs/000.bmp imgs/ 1 1 1 examples/` (one iteration in rgb-sequence-mode)<br>
![exmaple 1](examples/out_rgb_sequence.gif)<br>
`./zeitkompressor imgs/000.bmp imgs/ 4 1 0 examples/out_rgb.bmp` (four iterations in rgb-mode)<br>
![exmaple 1](examples/out_rgb.bmp)<br>
`./zeitkompressor imgs/000.bmp imgs 1 0 0 examples/out_gray.bmp` (one iteration in grayscale-mode)<br>
![exmaple 2](examples/out_gray.bmp)<br>

#### Assumptions
three channel bitmap images (RGB)<br>
256 images in image sequence

#### Build
On Linux:<br>
For Linux: `make`<br>
For Windows: `make windows`

#### Releases
Static binaries are available for Linux and Windows at [Releases](https://github.com/plensch/tempomap/releases)

#### Dependencies
stb_image.h and stb_image_write.h from the awesome [stb libraries](https://github.com/nothings/stb) (included in repo)

#### Icon
[Compress by Gregor Cresnar from the Noun Project](https://thenounproject.com/search/?q=compress&i=3560877)

