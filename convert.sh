chmod +x convert.sh

ulimit -s unlimited

rm frames/frame*.bmp

make

ffmpeg -ss 00:00:00 -t 00:00:06 -i test_film.mp4 -r 60.0 frames/frame%4d.bmp

for i in $(seq -f "%04g" 1 290)
do
    echo $i
    ./filter "frames/frame$i.bmp"
done

ffmpeg -r 60 -f image2 -s 1920x1080 -i frames/frame%04d.bmp -vcodec libx264 -crf 25  -pix_fmt yuv420p test_wynik.mp4
