echo "Qual cliente você quer rodar?"
read option

for i in `seq 1 30`;do
	echo "Executando client$option: iteração $i..."
	./client$option 127.0.0.1 &
	sleep 2s
done

exit 0
