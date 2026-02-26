library(Rcpp)

sourceCpp("src/auc.cpp")

scores <- c(0.9, 0.8, 0.75, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0.05)
labels <- c(  1,   0,    1,   1,   0,   1,   0,   0,   1,    0)


auc <- AUC(scores, labels)
cat("AUC =", auc, "\n")  

# --- Optional: compare against R's own pROC package ---
install.packages("pROC")
library(pROC)
auc_proc <- as.numeric(auc(labels, scores, direction="<"))
cat("pROC AUC =", auc_proc, "\n")
