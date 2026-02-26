#include <Rcpp.h>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace Rcpp;

// [[Rcpp::export]]
double AUC(NumericVector scores_r, IntegerVector labels_r) {
    int n = scores_r.size();

    std::vector<double> scores(scores_r.begin(), scores_r.end());
    std::vector<int>    labels(labels_r.begin(), labels_r.end());

    std::vector<int> sorted_indices(n);
    std::iota(sorted_indices.begin(), sorted_indices.end(), 0);
    std::sort(sorted_indices.begin(), sorted_indices.end(),
              [&](int i1, int i2) { return scores[i1] > scores[i2]; });

    int P = 0, N = 0;
    for (int l : labels)
        (l == 1 ? P : N)++;

    if (P == 0 || N == 0) Rcpp::stop("Need both positive and negative samples.");

    // Build full TPR/FPR curve
    std::vector<double> tpr, fpr;
    tpr.push_back(0.0);
    fpr.push_back(0.0);

    int tp = 0, fp = 0;
    for (int i : sorted_indices) {
        if (labels[i] == 1) tp++;
        else                fp++;
        tpr.push_back((double)tp / P);
        fpr.push_back((double)fp / N);
    }

    double auc = 0.0;
    for (int i = 1; i < (int)tpr.size(); i++) {
        double dfpr = fpr[i] - fpr[i-1];
        double avg_tpr = 0.5 * (tpr[i] + tpr[i-1]);
        auc += dfpr * avg_tpr;
    }

    return auc;
}