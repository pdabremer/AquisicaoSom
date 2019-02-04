# AquisicaoSom
Capturar som do apito


[b]Para utilizar o alsa-sound-example[/b]

1. sudo apt-get install libasound2-dev

2. git clone https://gist.github.com/104902.git /home/<usuário>/Nao/Alsa/

3. aplay -l		% pegar valores x="placa" e y="dispositivo secundário"

4. cd /home/<usuário>/Nao/Alsa/

5. gcc -o alsa-record-example alsa-record-example.c -lasound

6. ./alsa-record-example hw:1,0 % substitui pelos valores hw:x,y


[b]Para gravar e reproduzir[/b]

1. gcc -o gravacao gravacao.c -lasound

2. ./gravacao > audio.raw

3. gcc -o reproducao reproducao.c -lasound

4. ./reproducao < audio.raw
