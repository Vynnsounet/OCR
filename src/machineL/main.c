#include "matrix/matrix.h"
#include "matrix/ops.h"
#include "neural/activations.h"
#include "neural/nn.h"
#include "util/img.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int predict_number_on_file(NeuralNetwork *net, char *filename)
{
    Img *pic = file_to_img(filename);
    int r = predict_number(net, pic);
    img_free(pic);
    return r;
}

int main(int argc, char **argv)
{
    srand(time(NULL));

    // TRAINING
    int number_imgs = 1000;
    Img **imgs = csv_to_imgs("./data/mnist_test.csv", number_imgs);
    NeuralNetwork *net = network_create(784, 300, 10, 0.1);
    network_train_batch_imgs(net, imgs, number_imgs);
    network_save(net, "testing_net");

    // PREDICTING
    // int number_imgs = 3000;
    // Img **imgs = csv_to_imgs("data/mnist_test.csv", number_imgs);
    // NeuralNetwork *net = network_load("testing_net");
    // printf("%s\n", argv[1]);
    // printf("%d\n", predict_number_on_file(net, argv[1]));
    // double score = network_predict_imgs(net, imgs, 1000);
    // printf("Score: %1.5f\n", score);
    //
    imgs_free(imgs, number_imgs);
    network_free(net);
    return 0;
}
