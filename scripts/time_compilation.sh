start_time=`date +%s`
make re && echo run time is $(expr `date +%s` - $start_time) s