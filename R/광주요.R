library(data.table)
library(dplyr)
library(devtools)
library(readAny)
library("reshape2")
# data <- read.csv("df_190723.csv", head = TRUE)
###############################################################################################
# new_data <- read.any("Template.csv", header = TRUE) #추가할데이터
# new_data <- new_data[,c(-46)]
# new_data <- new_data[,c(-2:-25)]
# new_data <- new_data[,c(-5)]
# new_data <- new_data[,c(-13:-20)]
# 
# names(new_data) <- c("date","code","name","spec","quantity","unit","price_vat","price","foreing_price","supply_price","vat","briefs")
# 
# new_data <- new_data[!grepl("택배비\t", new_data$name),]
# new_data <- new_data[!grepl("포장비\t", new_data$name),]
# new_data <- new_data[!grepl("MC03\t", new_data$code),] #MC-온백자 도화문
# new_data <- new_data[!grepl("MC09\t", new_data$code),] #MC-계영배
# new_data <- new_data[!grepl("BG", new_data$code),]
# new_data <- new_data[!grepl("BC", new_data$code),]
# new_data <- new_data[!grepl("MR", new_data$code),]
# new_data <- new_data[!grepl("BS", new_data$code),]
# new_data <- new_data[!grepl("MF", new_data$code),]
# new_data <- new_data[!grepl("MC", new_data$code),]
# new_data <- new_data[!grepl("BP", new_data$code),]
# new_data <- new_data[!grepl("MW", new_data$code),]
# new_data <- new_data[!grepl("BH", new_data$code),]
# new_data <- new_data[!grepl("MS", new_data$code),]
# new_data <- new_data[!grepl("MT", new_data$code),]
# new_data <- new_data[!grepl("CP04", new_data$code),]
# new_data <- new_data[!grepl("SP", new_data$code),]
# 
################################################################################################# 
# data <- rbind(data,new_data)
# 
# write.csv(data, file ='data.csv',row.names = F)

kwangjuyo <- function(data, total_sale ,index, alpha){
  library(data.table)
  library(forecast)
  library(dplyr)
  library("reshape2")
  ####################################################################################
  important_item = NULL
  
  data$name = as.character(data$name)
  
  data$date = as.character(data$date)
  data$date = as.Date(data$date, "%Y%m%d")
  data <- subset(data, date<="2018-12-31")
  
  data$quantity <- as.numeric(gsub(",","",data$quantity))
  data$supply_price <- as.numeric(gsub(",","",data$supply_price))
  data$vat <- as.numeric(gsub(",","",data$vat))
  data$price <- as.numeric(gsub(",","",data$price))
  data$price = data$supply_price + data$vat
  
  data$code = as.character(data$code)
  data$code <- gsub("\t", "", data$code)
  code_list = unique(data$code)
  #################################################################################
  #전체제품의 평균매출액 = 전체 매출액 / 전체 물품개수
  sum(data$price, na.rm = T) / length(code_list) 
  
  for (n in 1:length(code_list)) {
    total = NULL
    row = NULL
    forecast_Month = NULL
    forecast_total_temp = NULL
    forecast_total = NULL
    #############################################################################################################
    #item_data = data[data$code == code_list[1],] #검산용 하나
    item_data = data[data$code == code_list[n],] #전체용
    
    item_data_subset <- subset(item_data, select=c(name, date, quantity,price))
    item_data_subset = item_data_subset[complete.cases(item_data_subset[,c("quantity")]),]
    item_data_subset = item_data_subset[complete.cases(item_data_subset[,c("price")]),]
    #특정칼럼에 NA가 있으면 그행을 제거함. NA.omit은 모든 칼럼에 NA가 있는 행을 제거함.
    
    if(sum(item_data_subset$price)>total_sale){ #첫번째 조건)input으로 받은 금액보다 큰 아이템만 수행하게 함
      ##################################################################################################
      #판매가 없었던 달에 0을 추가하는 작업
      pre_cast = NULL
      de_cast = NULL
      
      start <- as.Date(item_data_subset$date[1])
      end <- as.Date(item_data_subset$date[length(item_data_subset$date)])
      
      date <- seq(from=start, to=end, by=1)
      date <- as.factor(date)
      quantity <- rep(0, length.out=length(date))
      temp <- data.frame(date, quantity)          #모든날짜에 대한 판매액0 이라는 두칼럼을 만듬
      
      item_data_subset_noname = subset(item_data_subset, select = c(date,quantity))
      pre_cast <- rbind(item_data_subset_noname, temp)
      
      #date 데이터형식 날짜로 통일
      pre_cast$date <- as.Date(pre_cast$date)
      pre_cast$quantity <- as.numeric(pre_cast$quantity)
      de_cast <- dcast(pre_cast, date~., value.var = "quantity", sum)
      colnames(de_cast) = c("date", "total_sales")
      #드디어 완성된 데이터프레임
      ######################################################################################
      reshape_data_YM <- setDT(de_cast)[, .(total = sum(total_sales)), 
                                        by = .(year = year(date), month = months(date))]
      reshape_data_YM$month <- as.numeric(factor(reshape_data_YM$month, 
                                                 level=c("1월", "2월", "3월", "4월", "5월", "6월", "7월", "8월", "9월", "10월", "11월", "12월"))) 
      ##reshape_data의 month 칼럼이 1월,2월 이런 형식을 1,2,3 숫자로만 표시되도록 형식 변경
      
      reshape_data_Y <- setDT(de_cast)[, .(total = sum(total_sales)),
                                       by = .(year = year(date))]
      
      reshape_data_M <- setDT(de_cast)[, .(total = sum(total_sales)),
                                       by = .(month = months(date))]
      
      reshape_data_M$month <- as.numeric(factor(reshape_data_M$month, 
                                                level=c("1월", "2월", "3월", "4월", "5월", "6월", "7월", "8월", "9월", "10월", "11월", "12월"))) 
      ##reshape_data의 month 칼럼이 1월,2월 이런 형식을 1,2,3 숫자로만 표시되도록 형식 변경
      
      #############################################################################################################
      if(length(which(reshape_data_YM$total==0)) < nrow(reshape_data_YM)/2 && nrow(reshape_data_M)==12){
        #두번째 조건)해당아이템의 전체 판매기간 중 판매가 없었던 달이 절반을 넘거나 AND 만 1년째 팔지 못한 제품을 걸러내는 조건
        #############################################################################################################
        #Month 그래프(월평균 판매개수, 8년치로 나눈 평균임)
        count_year = 0 #2011-01~2018-11까지 팔렸다 하면 1월부터 11월은 8개씩 있지만 12월은 7개 뿐이라 데이터길이가 다름
        sum_month = 0   #해당월의 데이터하
        
        for (i in 1:12){
          for (j in 1:nrow(reshape_data_YM)){
            if(reshape_data_YM$month[j] == i) {
              count_year = count_year + 1
              sum_month = sum_month+reshape_data_YM$total[j]
              #년월별로 되있는 데이터를 월별로 합계
            }
          }#여기서 YM데이터 한바뀌 싹 돔
          sum_month = round(sum_month/count_year)
          total <- c(total, sum_month)
          count_year = 0
          sum_month = 0
          #한달씩 여기서 끝
        }
        
        reshape_M.ts <- ts(total) #결국 total은 평균법이라고 볼 수 있음
        row <- cbind(item_data_subset$name[1],code_list[n],t(total)) #길이 14
        Sys.sleep(1)
        #############################################################################################################
        plot(reshape_M.ts , 
             main = paste0(reshape_data_YM$year[length(reshape_data_YM$year)]-reshape_data_YM$year[1]+1,"년치",
                           "(",reshape_data_YM$year[1],".",reshape_data_YM$month[1],"~", 
                           reshape_data_YM$year[length(reshape_data_YM$year)],".",reshape_data_YM$month[length(reshape_data_YM$month)],") ",
                           "월별 평균판매갯수\n", item_data_subset$name[1], "(",code_list[n],")"),
             type ="o", cex = 0.7, xlab = "t(월)", ylab = "y(평균판매개수)",
             ylim = c(min(reshape_M.ts)*0.5,max(reshape_M.ts)*1.5), axes=F
        )
        legend("topleft",legend=c(paste0("월평균판매개수 = ", round(sum(reshape_M.ts)/12))), cex = 0.6)
        legend("topright",legend=c("실데이터","이동평균","지수가중", "회귀", "ARIMA"),fill=c(1:5), cex=0.7)
        
        axis(2)
        axis(1, at = c(1:12))
        grid(24)
        text(x=c(1:12), y = reshape_M.ts, labels = reshape_M.ts, cex =0.7)
        #############################################################################################################
        #각각기법을 통해 예측치 도출을 위한 부분
        y = NULL
        Na_col = rbind("-", "-","-","-")
        name_col = rbind("이동평균","지수평활","선형회귀","ARIMA")
        
        for(i in 1:12){
          for(j in 1:nrow(reshape_data_YM)){
            if(i==reshape_data_YM$month[j])
              y = c(y,reshape_data_YM$total[j])
          }
          
          if(length(y)>=3){  
            #세번째 조건)월데이터가 3개도 안되면 예측이 의미가 없음
            yhat_next_MA = round(sum(y[length(y):(length(y)-index+1)]) / index)
            text(x=i, y=yhat_next_MA, labels = yhat_next_MA, cex = 0.7,col=2)
            
            yhat_ES = NULL
            yhat_ES <- c(yhat_ES,(alpha*y[1]+(1-alpha)*y[1]))
            for (k in 2:length(y)){
              temp= alpha*y[k-1]+(1-alpha)*yhat_ES[k-1]
              yhat_ES<-c(yhat_ES,temp)
            }
            yhat_next_ES = round(alpha*y[length(y)-1] + (1-alpha)*yhat_ES[length(yhat_ES)])
            text(x=i, y=yhat_next_ES, labels = yhat_next_ES, cex = 0.7,col=3)
            
            linear <- lm(y~c(1:length(y)))
            betta0 <- as.numeric(linear$coefficients[1])
            betta1 <- as.numeric(linear$coefficients[2])
            yhat_next_LR = round(betta0+betta1*(length(y)+1))
            text(x=i, y = yhat_next_LR, labels = yhat_next_LR, cex =0.7,col=4)
            
            if(length(which(y==0)) < length(y) && length(y)>4){
              #네번째 조건 ARIMA가 안먹는 경우를 막으려고
              t.arima <- arima(y)
              yhat_next_AR = forecast(t.arima, h=1)
              yhat_next_AR = round(yhat_next_AR$mean)
              text(x=i, y = yhat_next_AR, labels = yhat_next_AR, cex =0.7,col=5)
            }
            else  yhat_next_AR= "-"
            
          }#세번째 조건 if에 대한 괄호
          
          
          if(length(y) < 3){
            yhat_next_MA="-"
            yhat_next_ES="-"
            yhat_next_LR="-"
            yhat_next_AR="-"
          }
          
          #####################################################################
          
          forecast_Month = rbind(yhat_next_MA, yhat_next_ES, yhat_next_LR, yhat_next_AR)#한달에 대한 예측치 모음
          y = NULL
          forecast_total_temp = cbind(forecast_total_temp, forecast_Month)
        }#예측치 구하는 i(12달)에 대한 괄호 
        #######################################################################
        forecast_total = cbind(Na_col,name_col,forecast_total_temp)# 4X14
        forecast_total = rbind(row, forecast_total)
        forecast_total_temp = NULL
      }#두번째 조건 if에 대한 괄호
      important_item <- rbind(important_item, forecast_total)
      
    }#첫번째 조건 if에 대한 괄호
    
    
  }#모든 코드를 도는 for문에 대한 괄호
  
  colnames(important_item) = c("품명","코드/예측방법", paste0(c(1:12),"월"))
  return(important_item)
}#전체 함수에 대한 괄호 마무리 

data2 <- read.csv("df_190723.csv", head = TRUE)
im_item <- kwangjuyo(data2,50000000,3,0.9) #이동평균인덱스는 3이상, 지수평활법 알파는 0~1까지
write.csv(im_item,file="important_item.csv", row.names = F)




data <- read.csv("C:/Users/pc/Desktop/kwangjuyo/df_190723.csv", head = TRUE)



kwangjuyo_12month <- function(data, input_code){
  # library(data.table)
  # library(forecast)
  # library(dplyr)
  # library("reshape2")
  ####################################################################################
  important_item = NULL
  
  data$name = as.character(data$name)
  
  data$date = as.character(data$date)
  data$date = as.Date(data$date, "%Y%m%d")
  data <- subset(data, date<="2018-12-31")
  
  data$quantity <- as.numeric(gsub(",","",data$quantity))
  data$supply_price <- as.numeric(gsub(",","",data$supply_price))
  data$vat <- as.numeric(gsub(",","",data$vat))
  data$price <- as.numeric(gsub(",","",data$price))
  data$price = data$supply_price + data$vat
  
  data$code = as.character(data$code)
  data$code <- gsub("\t", "", data$code)
  code_list = unique(data$code)
  
  #item_data = data[data$code == "5000Q0001",]
  #item_data = data[data$code == code_list[1],] #검산용 하나
  item_data = data[data$code == input_code,] 
  
  item_data_subset <- subset(item_data, select=c(name, date, quantity,price))
  item_data_subset = item_data_subset[complete.cases(item_data_subset[,c("quantity")]),]
  item_data_subset = item_data_subset[complete.cases(item_data_subset[,c("price")]),]
  #특정칼럼에 NA가 있으면 그행을 제거함. NA.omit은 모든 칼럼에 NA가 있는 행을 제거함.
  
  ##################################################################################################
  #판매가 없었던 달에 0을 추가하는 작업
  start <- as.Date(item_data_subset$date[1])
  end <- as.Date(item_data_subset$date[length(item_data_subset$date)])
  
  date <- seq(from=start, to=end, by=1)
  date <- as.factor(date)
  quantity <- rep(0, length.out=length(date))
  temp <- data.frame(date, quantity)          #모든날짜에 대한 판매액0 이라는 두칼럼을 만듬
  
  item_data_subset_noname = subset(item_data_subset, select = c(date,quantity))
  pre_cast <- rbind(item_data_subset_noname, temp)
  
  #date 데이터형식 날짜로 통일
  pre_cast$date <- as.Date(pre_cast$date)
  pre_cast$quantity <- as.numeric(pre_cast$quantity)
  de_cast <- dcast(pre_cast, date~., value.var = "quantity", sum)
  colnames(de_cast) = c("date", "total_sales")
  #드디어 완성된 데이터프레임
  ######################################################################################
  reshape_data_YM <- setDT(de_cast)[, .(total = sum(total_sales)), 
                                    by = .(year = year(date), month = months(date))]
  reshape_data_YM$month <- as.numeric(factor(reshape_data_YM$month, 
                                             level=c("1월", "2월", "3월", "4월", "5월", "6월", "7월", "8월", "9월", "10월", "11월", "12월"))) 
  ##reshape_data의 month 칼럼이 1월,2월 이런 형식을 1,2,3 숫자로만 표시되도록 형식 변경
  
  reshape_data_Y <- setDT(de_cast)[, .(total = sum(total_sales)),
                                   by = .(year = year(date))]
  
  reshape_data_M <- setDT(de_cast)[, .(total = sum(total_sales)),
                                   by = .(month = months(date))]
  
  reshape_data_M$month <- as.numeric(factor(reshape_data_M$month, 
                                            level=c("1월", "2월", "3월", "4월", "5월", "6월", "7월", "8월", "9월", "10월", "11월", "12월"))) 
  ##reshape_data의 month 칼럼이 1월,2월 이런 형식을 1,2,3 숫자로만 표시되도록 형식 변경
  
  #############################################################################################################
  #Month 그래프(월평균 판매개수, 8년치로 나눈 평균임)
  count_year <- 0
  sum_month = 0
  total = NULL
  
  for (i in 1:12){
    for (j in 1:nrow(reshape_data_YM)){
      if(reshape_data_YM$month[j] == i) {
        count_year = count_year + 1
        sum_month = sum_month+reshape_data_YM$total[j]
      }
    }
    sum_month = round(sum_month/count_year)
    total <- c(total, sum_month)
    count_year = 0
    sum_month = 0
  }
  
  reshape_M.ts <- ts(total)
  #############################################################################################################
  y=NULL
  x=NULL
  forecast_total = NULL
  for(i in 1:12){
    for(j in 1:nrow(reshape_data_YM)){
      if(i==reshape_data_YM$month[j]){
        y = c(y,reshape_data_YM$total[j])
        x = c(x,reshape_data_YM$year[j])
      }
    }
    
    ###########################################
    plot(y=y ,x=x, main = paste0(i,"월 판매갯수\n", item_data_subset$name[1], "(",input_code,")"),
         type ="o", cex = 0.7, xlab = "t(연)", ylab = "y(판매개수)", 
         ylim = c(min(y)*0.5,max(y)*1.5),axes=F 
         
    )
    
    legend("topleft",legend=c(paste0("월평균판매개수 = ", round(reshape_M.ts[i]))), cex = 0.6)
    legend("topright",legend=c("실데이터", "회귀","ARIMA"),fill=c(1:3), cex=0.7)
    
    axis(2)
    axis(1,at= c(as.numeric(reshape_data_YM$year[i]) : as.numeric(reshape_data_YM$year[j])))
    grid(24)
    
    text(x=x, y = y, labels = y, cex =1.5)
    ############################################################################################################
    
    if(length(y)>=3){
      
      linear <- lm(y~x)
      yhat = linear$fitted.values
      lines(x=x, y=yhat, col=2)#추정치들
      
      t.arima <- arima(y)
      yhat = round(fitted(t.arima))
      lines(x=x, y=yhat, col=3)
    }
    
    y = NULL
    x = NULL
  }
  
  
}






kwangjuyo_12month(data,"9500Z0443")
