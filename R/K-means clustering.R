library(ggplot2)
library(readxl)

locate <- read_excel("locate_seoul_2.xlsx")
data_seoul <- data.frame(locate$x, locate$y)

data_seoul.kmeans <- kmeans(data_seoul, centers = 2, iter.max = 10000)

data_seoul.kmeans$centers

data_seoul$cluster <- as.factor(data_seoul.kmeans$cluster)
qplot(locate.y, locate.x, colour=cluster, data=data_seoul)

write.csv(data_seoul.kmeans$centers, file="data_seoul_center.csv", row.names = F)
#-------------------------------------------------------------------------------------
locate <- read_excel("locate_gang_2.xlsx")
data_gang <- data.frame(locate$x, locate$y)

data_gang.kmeans <- kmeans(data_gang, centers = 2, iter.max = 10000)

data_gang.kmeans$centers

data_gang$cluster <- as.factor(data_gang.kmeans$cluster)
qplot(locate.y, locate.x, colour=cluster, data=data_gang)

write.csv(data_gang.kmeans$centers, file="data_gang_center.csv", row.names = F)
#-------------------------------------------------------------------------------------
locate <- read_excel("locate_jun_2.xlsx")
data_jun <- data.frame(locate$x, locate$y)

data_jun.kmeans <- kmeans(data_jun, centers = 2, iter.max = 10000)

data_jun.kmeans$centers

data_jun$cluster <- as.factor(data_jun.kmeans$cluster)
qplot(locate.y, locate.x, colour=cluster, data=data_jun)

write.csv(data_jun.kmeans$centers, file="data_jun_center.csv", row.names = F)
#-------------------------------------------------------------------------------------
locate <- read_excel("locate_bu_3.xlsx")
data_bu <- data.frame(locate$x, locate$y)

data_bu.kmeans <- kmeans(data_bu, centers = 3, iter.max = 10000)

data_bu.kmeans$centers

data_bu$cluster <- as.factor(data_bu.kmeans$cluster)
qplot(locate.y, locate.x, colour=cluster, data=data_bu)

write.csv(data_bu.kmeans$centers, file="data_bu_center.csv", row.names = F)
#-------------------------------------------------------------------------------------
locate <- read_excel("locate_jeju_1.xlsx")
data_jeju <- data.frame(locate$x, locate$y)

data_jeju.kmeans <- kmeans(data_jeju, centers = 1, iter.max = 10000)

data_jeju.kmeans$centers

data_jeju$cluster <- as.factor(data_jeju.kmeans$cluster)
qplot(locate.y, locate.x, colour=cluster, data=data_jeju)

write.csv(data_jeju.kmeans$centers, file="data_jeju_center.csv", row.names = F)
#-------------------------------------------------------------------------------------
locate <- read_excel("center.xlsx")
data_center <- data.frame(locate$locate.x, locate$locate.y)

data_center.kmeans <- kmeans(data_center, centers = 6, iter.max = 10000)

data_center.kmeans$centers

data_center$cluster <- as.factor(data_center.kmeans$cluster)
qplot(locate.locate.y, locate.locate.x, colour=cluster, data=data_center)

write.csv(data_center.kmeans$centers, file="data_center.csv", row.names = F)
-------------------------------------------------------------------------------------