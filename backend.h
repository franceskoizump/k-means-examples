#ifndef BACKEND
#define BACKEND

#include <QObject>
#include <QString>
#include <iostream>
#include <vector>

class BackEnd : public QObject
{
    Q_OBJECT
public:
    explicit BackEnd(QObject *parent = nullptr);

private:
    std::vector<int> x_coords;
    std::vector<int> y_coords;

    std::vector<double> clusters_lengths;

    std::vector<int> x_centers;
    std::vector<int> y_centers;
    int last_ind = 0;
    double standart_deviation = 1.0;


public:
    Q_INVOKABLE inline void addPoint(int x, int y){
        x_coords.push_back(x);
        y_coords.push_back(y);
    }
    Q_INVOKABLE inline void clearAllPoints(){
        x_coords.clear();
        y_coords.clear();
    }
    Q_INVOKABLE inline void addCenter(int x, int y){
        x_centers.push_back(x);
        y_centers.push_back(y);
    }

    Q_INVOKABLE void solve();
    Q_INVOKABLE void solve_k_med();
    Q_INVOKABLE void solve_C_means();

    Q_INVOKABLE int getNewCenterX();
    Q_INVOKABLE int getNewCenterY();
    Q_INVOKABLE double getLengthCenter();
    Q_INVOKABLE void inc();
    Q_INVOKABLE inline void clearPoints(){
        x_coords = std::vector<int>();
        y_coords = std::vector<int>();
        last_ind = 0;
    }
    Q_INVOKABLE inline void clearCenters(){
        x_centers = std::vector<int>();
        y_centers = std::vector<int>();
        last_ind = 0;
    }
    Q_INVOKABLE inline void setDisp(double d){
        standart_deviation = d;
    }
};

#endif
