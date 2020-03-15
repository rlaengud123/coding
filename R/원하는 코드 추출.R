data <- read.csv("df_190723.csv", head = TRUE)

df <- data[1,]
df <- df[-1,]


count = 0
for (i in 1:nrow(data)){
  if (data$code[i] == "9500Z0410\t"){
    df[count,] <- data[i,]
    count = count + 1
  }
}

df$supply_price <- gsub(",","", df$supply_price)
df$supply_price <-as.numeric(df$supply_price)
sum(df$supply_price)

write.csv(df, file ='9500Z0410.csv',row.names = F)


