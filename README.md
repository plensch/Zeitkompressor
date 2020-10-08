# tempomap
Maps an image sequence to a canvas using the pixel values of a grayscale map as time reference.

##### Usage
`./tempomap [map] [image sequence] [iterations] [out]`

##### Example
`./tempomap map.bmp image_sequence 4 out.bmp`

##### Assumptions
three channel bitmap images (RGB)<br>
256 images in image sequence

#### Build
`make safe`<br/>
or <br/>
`make fast` around 20% faster, thanks to gcc

#### Dependencies
stb_image.h and stb_image_write.h from the awesome [stb libraries](https://github.com/nothings/stb) (included in repo).

