#Test script by Zhuoran Sun
spec="jabberwocky_spec"
port=9090
num=5
#Run server
echo "Starting sever."
./server $spec $port &
#Stores the pid of the server
pid=$!
#Wait a second for the server to set up.
sleep 1
#Print start time
date +"%T.%N"
#Run num clients
echo "Strating $num clients"
for ((i=0;i<$num;i++)); do
./client localhost $port &
done
#Wait until server has finished processing.
wait $pid
#Print end time
date +"%T.%N"
#Check difference
echo "Showing differences (if any)"
diff jabberwocky jabberwocky_og
