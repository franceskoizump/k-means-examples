#include "backend.h"
#include <limits>
#include <cmath>

BackEnd::BackEnd(QObject *parent) :
    QObject(parent)
{
}

void BackEnd::solve() {
    std::vector<int> prev_center_y;
    std::vector<int> prev_center_x;
    std::vector<double> prev_lens;
    while (prev_center_y != y_centers || prev_center_x != x_centers || prev_lens != clusters_lengths) {
        prev_center_x = std::vector<int>(x_centers);
        prev_center_y = std::vector<int>(y_centers);
        prev_lens = std::vector<double>(clusters_lengths);
        std::vector<int> clusters(x_coords.size());
        int choosen = 0;
        for (size_t i = 0; i < x_coords.size(); i++) {
            double cluster_length = std::numeric_limits<double>::max();
            double x = x_coords[i];
            double y = y_coords[i];
            for (size_t j = 0; j < x_centers.size(); j++) {
                double mx = x_centers[j];
                double my = y_centers[j];
                double len = sqrt(pow(mx - x, 2) + pow(my - y, 2));
                if (cluster_length > len) {
                    choosen = j;
                    cluster_length = len;
                }
            }
            clusters[i] = choosen;
            choosen = 0;
        }
        std::vector<int> clusters_size(x_centers.size());
        clusters_lengths.resize(x_centers.size());
        std::fill(clusters_lengths.begin(), clusters_lengths.end(), 0);
        std::fill(x_centers.begin(), x_centers.end(), 0);
        std::fill(y_centers.begin(), y_centers.end(), 0);
        for (size_t j = 0; j < x_coords.size(); j++) {
            clusters_size[clusters[j]]++;
            x_centers[clusters[j]] += x_coords[j];
            y_centers[clusters[j]] += y_coords[j];
        }
        for (size_t i = 0; i < clusters_size.size(); i++) {
            if (clusters_size[i] != 0) {
                x_centers[i] /= clusters_size[i];
                y_centers[i] /= clusters_size[i];
            }
        }
        for (size_t j = 0; j < x_coords.size(); j++) {
            double len = sqrt(1.0*(pow(x_centers[clusters[j]] - x_coords[j], 2) + pow(y_centers[clusters[j]] - y_coords[j], 2)));
            clusters_lengths[clusters[j]] = std::max(clusters_lengths[clusters[j]], len);
        }
        last_ind = 0;
    }
}

void BackEnd::solve_k_med() {
    std::vector<int> prev_center_y;
    std::vector<int> prev_center_x;
    std::vector<double> prev_lens;
    while (prev_center_y != y_centers || prev_center_x != x_centers || prev_lens != clusters_lengths) {
        prev_center_x = std::vector<int>(x_centers);
        prev_center_y = std::vector<int>(y_centers);
        prev_lens = std::vector<double>(clusters_lengths);
        std::vector<int> clusters(x_coords.size());
        int choosen = 0;
        for (size_t i = 0; i < x_coords.size(); i++) {
            double cluster_length = std::numeric_limits<double>::max();
            double x = x_coords[i];
            double y = y_coords[i];
            for (size_t j = 0; j < x_centers.size(); j++) {
                double mx = x_centers[j];
                double my = y_centers[j];
                double len = (abs(mx - x) + abs(my - y));
                if (cluster_length > len) {
                    choosen = j;
                    cluster_length = len;
                }
            }
            clusters[i] = choosen;
            choosen = 0;
        }
        std::vector<int> clusters_size(x_centers.size());
        clusters_lengths.resize(x_centers.size());
        std::fill(clusters_lengths.begin(), clusters_lengths.end(), 0);
        std::fill(x_centers.begin(), x_centers.end(), 0);
        std::fill(y_centers.begin(), y_centers.end(), 0);
        /*for (size_t j = 0; j < x_coords.size(); j++) {
            clusters_size[clusters[j]]++;
            x_centers[clusters[j]] += x_coords[j];
            y_centers[clusters[j]] += y_coords[j];
        }
        for (size_t i = 0; i < clusters_size.size(); i++) {
            if (clusters_size[i] != 0) {
                x_centers[i] /= clusters_size[i];
                y_centers[i] /= clusters_size[i];
            }
        }*/
        //медианы
        for (size_t i = 0; i < x_centers.size(); i++) {
            std::vector<int> xs;
            std::vector<int> ys;
            for (size_t j = 0; j < clusters.size(); j++) {
                if ((int)i == clusters[j]) {
                    xs.push_back(x_coords[j]);
                    ys.push_back(y_coords[j]);
                }
            }
            std::sort(xs.begin(), xs.end());
            std::sort(ys.begin(), ys.end());
            size_t s = xs.size();
            if (s % 2 == 0 && s != 0) {
                x_centers[i] = (xs[s/2] + xs[s/2+1] ) / 2;
                y_centers[i] = (ys[s/2] + ys[s/2+1] ) / 2;
            } else if (s % 2 == 1) {
                x_centers[i] = xs[s/2];
                y_centers[i] = ys[s/2];
            }


        }
        for (size_t j = 0; j < x_coords.size(); j++) {
            double len = sqrt(1.0*(pow(x_centers[clusters[j]] - x_coords[j], 2) + pow(y_centers[clusters[j]] - y_coords[j], 2)));
            clusters_lengths[clusters[j]] = std::max(clusters_lengths[clusters[j]], len);
        }
        last_ind = 0;
    }
}

void BackEnd::solve_C_means() {
    //нормируем все элементы
    int xmax = std::max(*std::max_element(x_coords.begin(), x_coords.end()),
                        *std::max_element(x_centers.begin(), x_centers.end()));
    int ymax = std::max(*std::max_element(y_coords.begin(), y_coords.end()),
                        *std::max_element(y_centers.begin(), y_centers.end()));

    double prev_lose = std::numeric_limits<double>::max();
    double lose = prev_lose/2;
    auto N = [&](double x) {
      double s = std::exp(-x*x/2.0/standart_deviation/standart_deviation) / standart_deviation / sqrt(2.0 * 3.14159);
      return s;
    };
    auto dist = [&](int x1, int y1, int x2, int y2) {
      return std::sqrt(1.0*(std::pow((x1-x2)/(double)xmax, 2) + std::pow((y1-y2)/(double)ymax, 2)));
    };
    while (lose < prev_lose) {
        prev_lose = lose;
        std::vector<std::vector<double> > R(x_coords.size(), std::vector<double>(x_centers.size()));
        for (size_t i = 0; i < x_coords.size(); i++) {
            for (size_t j = 0; j < x_centers.size(); j++) {
                R[i][j] = N(dist(x_coords[i], y_coords[i], x_centers[j], y_centers[j]));
                double sum = 0;
                for (size_t k = 0; k < x_centers.size(); k++) {
                    sum += N(dist(x_coords[i], y_coords[i], x_centers[k], y_centers[k]));
                }
                R[i][j] /= sum;
            }
        }
        std::vector<int> clusters(x_coords.size(), 0);
        for (size_t i = 0; i < x_coords.size(); i++) {
            int choosen = 0;
            double prob = 0;
            for (size_t j = 0; j < x_centers.size(); j++) {
                if (R[i][j] > prob) {
                    choosen = j;
                    prob = R[i][j];
                    std::cout << prob << " " <<  R[i][j] << std::endl;
                }
            }
            clusters[i] = choosen;
        }

        for (size_t j = 0; j < x_centers.size(); j++) {
            double x_num = 0;
            double x_denum = 0;
            double y_num = 0;
            double y_denum = 0;
            for (size_t i = 0; i < x_coords.size(); i++) {
                if (clusters[i] == (int)j) {
                    x_num += R[i][j] * x_coords[i];
                    y_num += R[i][j] * y_coords[i];
                    x_denum += R[i][j];
                    y_denum += R[i][j];
                }
            }
            if (x_denum != 0 && y_denum != 0){
                x_centers[j] = x_num / x_denum;
                y_centers[j] = y_num / y_denum;
            } else {
                x_centers[j] = 0;
                y_centers[j] = 0;
            }
        }
        clusters_lengths.resize(x_centers.size());

        std::fill(clusters_lengths.begin(), clusters_lengths.end(), 0);
        for (size_t i = 0; i < x_coords.size(); i++) {
            double len = sqrt(1.0*(pow(x_centers[clusters[i]] - x_coords[i], 2) + pow(y_centers[clusters[i]] - y_coords[i], 2)));
            clusters_lengths[clusters[i]] = std::max(clusters_lengths[clusters[i]], len);
        }
        last_ind = 0;
        lose = 0;
        for (size_t i = 0; i < x_coords.size(); i++) {
            for (size_t j = 0; j < x_centers.size(); j++) {
                lose += dist(x_coords[i], y_coords[i], x_centers[j], y_centers[j]) * R[i][j];
            }
        }
    }
}


int BackEnd::getNewCenterX() {
    if (last_ind == (int)x_centers.size())
        return 0;
    return x_centers[last_ind];
}
int BackEnd::getNewCenterY() {
    if (last_ind == (int)y_centers.size())
        return 0;
    return y_centers[last_ind];
}
double BackEnd::getLengthCenter() {
    if (last_ind == (int)clusters_lengths.size())
        return 0;
    return clusters_lengths[last_ind];
}
void BackEnd::inc() {
    if (last_ind == (int)x_centers.size())
        last_ind = 0;
    last_ind++;
}


