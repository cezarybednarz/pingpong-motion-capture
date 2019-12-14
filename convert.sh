chmod +x convert.sh

mkdir frames
ulimit -s unlimited

rm frames/frame*.bmp

make

ffmpeg -ss 00:00:20 -t 00:00:23 -i test_film.mp4 -r 30.0 frames/frame%4d.bmp

for i in $(seq -f "%04g" 1 90)
do
    echo $i
    ./filter "frames/frame$i.bmp"
done
rm test_wynik.mp4
# maybe -crf 60, or change -r
ffmpeg -r 30 -f image2 -s 1920x1080 -i frames/frame%04d_converted.bmp -vcodec libx264 -crf 25  -pix_fmt yuv420p test_wynik.mp4

echo essa
