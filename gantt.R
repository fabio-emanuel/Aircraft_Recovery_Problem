plotGantt <- function(cam,data, period, res.col='acft_org',start.col='dept',end.col='arrv', od.col = 'od', voo.col = 'voo', 
		result.col = 'result',start2.col='dept_final',end2.col='arrv_final',acft_novo.col = 'acft_novo', mant.col = 'mant' , acft_bd.col = 'acft_bd', cnf.col = 'cnf', model.col = 'model', alt_flt.col = 'alt_flt')
{
  #slightly enlarge Y axis margin to make space for labels
  op <- par('mar')
  par(mar = op + c(0,1.2,0,0)) 
  

  #Plot Original Flights
  
  minval <- min(data[,start.col],na.rm=T)
  maxval <- max(data[,end.col],na.rm=T)
  if (period[2]>maxval)
	maxval <- period[2]*1.1
  
  
  configs <- unique(data[,cnf.col])
  
  for(j in 1:length(configs)) {
  
	op <- par('mar')
    par(mar = op + c(0,1.2,0,0)) 
	
	subset0 <- data[data[,cnf.col] == configs[j],]
	resources <- sort(unique(subset0[,res.col]),decreasing=T)
	
	
	
	pdf(paste(cam,"//orgplt_",subset0[j,model.col],".pdf",sep=""))
	
	plot(c(minval,maxval),
		   c(0.5,length(resources)+0.5),
		   type='n', xlab='Time (min)',ylab=NA,yaxt='n',main = subset0[j,model.col])
	  axis(side=2,at=1:length(resources),labels=resources,las=1,cex.axis = 0.9)
	  # recovery period lines
	  abline(v = period[1], col = "blue")
	  abline(v = period[2], col = "blue")
	  for (dia in 0:length(maxval%/%1440)) {
		abline(v = 1440*dia, col = "black",lty = 3) }
		
		
	
	for(i in 1:length(resources))
	{
		yTop <- i+0.1
		yBottom <- i-0.1
		subset <- data[data[,res.col] == resources[i],]
		for(r in 1:nrow(subset))
		{
		
			
		
			  #color <- res.colors[((i-1)%%length(res.colors))+1]
			  
			
			  
			  if (subset[r,mant.col]==1) {
				color <- 'black' }
			  else {
				color <- 'grey' }
				
			  if (subset[r,alt_flt.col] >= 1 ) {
			    voo <-paste(subset[r,voo.col],"(",subset[r,alt_flt.col],")",sep="") 
				color <- 'powderblue' }
			 else  { voo <-subset[r,voo.col] 
			 }
				
				
				
			  start <- subset[r,start.col]
			  end <- subset[r,end.col]
			  od <-subset[r,od.col]
			  
			  rect(start,yBottom,end,yTop,col=color)
			  center <- c(mean(c(start, end)), mean(c(yBottom)))
			  text(center[1], center[2]-0.1, labels = od, cex = 0.60)
			  center <- c(mean(c(start, end)), mean(c(yTop)))
			  text(center[1], center[2]+0.1, labels = voo, cex = 0.75)
		}
		par(mar=op) # reset the plotting margins
	}
    dev.off()
  }
  
  
  #Plot Modified Flights
  
  minval <- min(data[,start.col],na.rm=T)
  maxval <- max(data[,end.col],na.rm=T)
  if (period[2]>maxval)
	maxval <- period[2]*1.1
  
  
  configs <- unique(data[,cnf.col])
  
  for(j in 1:length(configs)) {
  
	subset0 <- data[data[,cnf.col] == configs[j],]
	resources <- sort(unique(subset0[,acft_novo.col]),decreasing=T)
	
		
	pdf(paste(cam,"//mdfplt_",subset0[j,model.col],".pdf",sep=""))
	
	plot(c(minval,maxval),
		   c(0.5,length(resources)+0.5),
		   type='n', xlab='Duration',ylab=NA,yaxt='n',main = subset0[j,model.col])
	  axis(side=2,at=1:length(resources),labels=resources,las=1,cex.axis=0.9)
	  # recovery period lines
	 abline(v = period[1], col = "blue")
	  abline(v = period[2], col = "blue")
	  for (dia in 0:length(maxval%/%1440)) {
		abline(v = 1440*dia, col = "black",lty = 3) }
		
		
	
	for(i in 1:length(resources))
	{
		yTop <- i+0.1
		yBottom <- i-0.1
		subset <- data[data[,acft_novo.col] == resources[i],]
		for(r in 1:nrow(subset))
		{
		
		  #color <- res.colors[((i-1)%%length(res.colors))+1]
		  
		  if (subset[r,result.col]==0) {
			color <- 'grey96'
			voo <-subset[r,voo.col]
			}
		  else {
		    color <- 'orchid4' 
			if (subset[r,alt_flt.col]>=1) {
				color <- 'powderblue' }
			voo <-paste(subset[r,voo.col]," ",toString(30*subset[r,result.col]),"'",sep="")
			}
		  
		  
		  if (subset[r,mant.col]==1) {
			color <- 'black' }
			
		  if (subset[r,acft_bd.col]==1) {
			color <- 'red' }
	      
		
		  #start <- subset[r,start.col]
		  #end <- subset[r,end.col]
		  od <-subset[r,od.col]
		  
		  #rect(start,yBottom,end,yTop,col=color)
		  #modified flight
			if (subset[r,result.col]!=-1) {
			  start <- subset[r,start2.col]
			  end <- subset[r,end2.col]
			  rect(start,yBottom,end,yTop,col=color)
			  center <- c(mean(c(start, end)), mean(c(yBottom)))
			  text(center[1], center[2]-0.1, labels = od, cex = 0.80)
			  center <- c(mean(c(start, end)), mean(c(yTop)))
			  text(center[1], center[2]+0.1, labels = voo, cex = 0.85)
			  center <- c(mean(c(start, end)), mean(c(yBottom)))
			  if ( subset[r,res.col] != subset[r,acft_novo.col])
				text(center[1], center[2]-0.25, labels = subset[r,res.col], cex = 0.90)
			}
		}
		par(mar=op) # reset the plotting margins
	}
	dev.off()
  }
  
  
  
  
  
}


#beg end
#720 240
#acft_org,hbt,dept,arrv,od,voo,result,dept_final,arrv_final,acft_novo,mant,acft_bd,cnf
#A320#4,70,485,555,CDG-MLH,7d145,1,515,585,A320#4,0,0,5


#cam <- "c://Users//fabiomorais//Desktop//Recovery//ap_recovery_2//Results//A01_6088570"

cam <- getwd()
arq <- paste(cam,"//trilhosR.txt",sep="")

period = read.table(arq,header = TRUE, nrows = 1 )
data = read.csv(arq,header = TRUE, skip = 2)

plotGantt(cam, data, period, res.col='acft_org',start.col='dept',end.col='arrv', od.col = 'od', voo.col = 'voo', 
		result.col = 'result',start2.col='dept_final',end2.col='arrv_final',acft_novo.col = 'acft_novo', mant.col = 'mant' , acft_bd.col = 'acft_bd', cnf.col = 'cnf', model.col = 'model', alt_flt.col = 'alt_flt')

