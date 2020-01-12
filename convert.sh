chmod +x convert.sh

mkdir frames
ulimit -s unlimited

rm frames/frame*.bmp

make

ffmpeg -ss 00:00:00 -t 00:00:03 -i film.mp4 -r 25.0 -vf scale=1080:1920 frames/frame%4d.bmp

FRAMES=90

for i in $(seq -f "%04g" 1 $FRAMES)
do
    echo $i
    ./filter "frames/frame$i.bmp" $i $FRAMES
done
rm wynik.mp4

ffmpeg -r 25 -f image2 -s 1080x1920 -i frames/frame%04d_converted.bmp -vcodec libx264 -crf 25  -pix_fmt yuv420p wynik.mp4

#ffmpeg -ss 0 -t 4 -i wynik.mp4 -vf "fps=30,scale=320:-1:flags=lanczos,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse" -loop 0 output.gif

echo essa
