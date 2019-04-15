/* Isso segura o código do erro que for retornado*/
int err; 

/* Handle do nosso dispositivo */
snd_pcm_t *pcm_handle = NULL;

/* O nome do dispositivo */
const char *device_name = "default"; 

/* Abre o dispositivo */
err = snd_pcm_open (&pcm_handle, device_name, SND_PCM_STREAM_PLAYBACK, 0); 
// 1- Variável que segura o valor de retorno das chamadas
// 2- Diz qual dispositivo o Alsa deve abrir (*device_name = "hw:0,0" ??? default serve por estar setado no sistema ou no arquivo asoundrc do usuário)
// 3- Direção do stream: PLAYBACK (tocar) ou CAPTURE (gravar)
// 4- Modo de abrir o dispositivo (com SND_PCM_NONBLOCK as chamadas snd_pcm_writei ou snd_pcm_writen não vão bloquear enquanto tiver espaço no buffer e automaticamente retorna erro de código EAGAIN)

/* Inicializa os dados */
 snd_pcm_hw_params_t *hw_params;

/* Chama essa função API pra alocar dinamicamente uma variável desse tipo */
 snd_pcm_hw_params_malloc (&hw_params);

/* Inicializa a estrutura dos parâmetros do hardware*/
 snd_pcm_hw_params_any(pcm_handle, hwparams);

unsigned int rrate = 44100;

/* Função para setar o modo de transferência*/
snd_pcm_hw_params_set_access (pcm_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
// RW (usando funções snd_pcm_write*) ou MMAP (escrevendo direto para um ponteiro de memória)
// NTERLEAVED (LL RR LL RR LL RR LL RR LL RR) NONINTERLEAVED LL LL LL LL LL RR RR RR RR RR)

/* Formato da amostra */ 
snd_pcm_hw_params_set_format (pcm_handle, hw_params, SND_PCM_FORMAT_S16_LE);

/* Taxa de amostragem */
snd_pcm_hw_params_set_rate_near (pcm_handle, hw_params, &rrate, NULL); 
// Muda valor da variável rrate para mostrar o valor real

/* Número de canais */
snd_pcm_hw_params_set_channels (pcm_handle, hw_params, 2);

/* Tamanho do buffer e do período */
snd_pcm_uframes_t buffer_size = 1024;
snd_pcm_uframes_t period_size = 64;


snd_pcm_hw_params_set_buffer_size_near (pcm_handle, hw_params, &buffer_size);
snd_pcm_hw_params_set_period_size_near (pcm_handle, hw_params, &period_size, NULL);
// Valores bem baixos diminuem o delay (valores em bytes, pode especificar também em nanosegundos usando os parâmetros do hardware "buffer_time" e "period_time")
// Outros parâmetros de hardware que podem ser mudados: referência de APIs http://www.alsa-project.org/alsa-doc/alsa-lib/group___p_c_m___h_w___params.html

/* Configurando os parâmetros */
snd_pcm_hw_params (pcm_handle, hw_params);

/* Limpando a estrutura */
snd_pcm_hw_params_free (hw_params);

/* Estutura dos parâmetros de software */
snd_pcm_sw_params_t *sw_params;

snd_pcm_sw_params_malloc (&sw_params);
snd_pcm_sw_params_current (pcm_handle, sw_params);

/* QUando ALSA começa a tocar */
snd_pcm_sw_params_set_start_threshold(pcm_handle, sw_params, buffer_size - period_size);
// Neste caso, esperar até quando o buffer estiver quase cheio

snd_pcm_sw_params_set_avail_min(pcm_handle, sw_params, period_size);
// Notificação assíncrona para o ALSA nos dizer quando devemos preencher o buffer com dados. Devemos escrever pelo menos amostras de period_size para o buffer sem bloquear
// Outros parâmetros de software: referência de APIs http://www.alsa-project.org/alsa-doc/alsa-lib/_2test_2pcm_8c-example.html

/* Aplica mudanças feitas no dispositivo */
snd_pcm_sw_params(pcm_handle, sw_params)

/* Limpa memória para não haver despejos */
snd_pcm_sw_params_free (sw_params);

/* Prepara o dispositivo para escrita */
snd_pcm_prepare (pcm_handle);

/* Escreve o primeiro pedaço de som para o dispositivo */
snd_pcm_writei (pcm_handle, MyBuffer, 2 * period_size);
// Se escolher usar o formato NONINTERLEAVED usar snd_pcm_writen
// "MyBuffer" é um ponteiro para qualquer dado que você quiser escrever pro dispositivo
// O último parâmetro é o tamanho em bytes do dado à ser escrito (duas vezes maior que o tamanho do período é bom porque menos dá buffer underrun erro de "broken pipe" e mais dá erro "file descriptor in a bad state")

/* Inicializando a notificação */
snd_async_handler_t *pcm_callback;

snd_async_add_pcm_handler(&pcm_callback, pcm_handle, MyCallback, NULL)
// 3- É a função de callback
// 4- void pointer para qualquer dado que quiser passar para o callback

/* Habilitando e começando os callbacks */
snd_pcm_start(pcm_handle);

// Aqui entra o reconhecimento do apito ???

/* Processando o callback e preenchendo o buffer */
void MyCallback(snd_async_handler_t *pcm_callback);
void MyCallback(snd_async_handler_t *pcm_callback) {
                snd_pcm_t *pcm_handle = snd_async_handler_get_pcm(pcm_callback);
  snd_pcm_sframes_t avail;
  int err;

  avail = snd_pcm_avail_update(pcm_handle);
  while (avail >= period_size) {
     snd_pcm_writei(pcm_handle, MyBuffer, period_size);
     avail = snd_pcm_avail_update(handle);
     }
 }
// Repreenche o buffer com certa frequência, adicionando novo stream de dados para manter o som tocando suavemente   


/* Acaba com as callbacks
// snd_async_del_handler (pcm_callback);
// snd_pcm_drop(pcm_handle);
// snd_pcm_close (pcm_handle);
