#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

// video metadata
const char* name = "badapple.mp4"; // put video in files/
const int width = 120;
const int height = 90;
const int pixels = width * height;
const int frames = 6572;

// print ascii art of a frame
void print_frame(FILE *in, FILE *out) {
    fseek(in, 14, SEEK_SET); // skip magic numbers and metadata
    int c;
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            c = fgetc(in);
            if (192 <= c && c <= 255)
                fputc('@', out);
            else if (0 <= c && c <= 63)
                fputc(' ', out);
            else if (64 <= c && c <= 127)
                fputc('.', out);
            else
                fputc('*', out);
        }
        fputc('\n', out);
    }
}

/*
    recommended settings:
    "editor.letterSpacing": 4
    "editor.lineHeight": 11
*/

// create slideshow in play
int main() {
    // extract all frames to files/ using ffmpeg
    char* a1[] = {"cd", "files", NULL};
    execvp(a1[0], a1);
    char* a2[] = {"ffmpeg", "-i", name, "%d.pgm", NULL};
    execvp(a2[0], a2);

    FILE* out = fopen("play", "w");
    FILE* frame;
    char path[15];
    struct timeval tv;
    int sleep;

    for (int i = 1; i <= frames; i++) {
        /* control speed
        // apporximately 4 fps
        gettimeofday(&tv, NULL);
        sleep = (((tv.tv_usec + 249999) / 250000) * 250000) - tv.tv_usec;
        if (sleep < 0)
            sleep = 0;
        usleep(sleep);
        */

        // process frame
        sprintf(path, "files/%d.pgm", i);
        frame = fopen(path, "r");
        print_frame(frame, out);
        fclose(frame);
        fflush(out);
    }

    fclose(out);
    return 0;
}

/* stitch frames into a video through terminal
// cd files
// ffmpeg -i badapple.mp4 %d.pgm

int main(int argc, char* argv[]) {
    FILE* out = fopen("play", "w");
    FILE* frame = fopen(argv[1], "r");
    print_frame(frame, out);
    fclose(frame);
    fclose(out);
    return 0;
}

// for i in {1..6572}; do ./a.out files/$i.pgm; sleep 0.25; screencapture -D 2 -x -r ./out/$i.png; done
// ffmpeg -framerate 30 -i %d.png -pix_fmt yuv420p out.mp4
*/
