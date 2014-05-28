sensever
========

O projeto consiste em 17 tablados com sensores piezo-elétricos.

<pre>
      Esquemático dos tablados
                _____
               /     \
         _____/   5   \_____
        /     \       /     \
       /  13   \_____/   9   \
       \       /     \       /
        \_____/   4   \_____/
        /     \       /     \
  _____/  12   \_____/   8   \_____
 /     \       /     \       /     \
/  17   \_____/   3   \_____/  15   \
\       /     \       /     \       /
 \_____/  11   \_____/   7   \_____/
 /     \       /     \       /     \
/  16   \_____/   2   \_____/  14   \
\       /     \       /     \       /
 \_____/  10   \_____/   6   \_____/
       \       /     \       /
        \_____/   1   \_____/
              \       /
               \_____/
</pre>

A leitura dos sensores é feita com 4 placas Arduino distribuidas entre os
tablados. Os Arduinos estão nas placas 2, 4, 14 e 16, de forma a otimizar
o caminho de comunicação do circuito. A placa de número 4 está ligada pela
USB ao PC que controla o circuito. A placa 4 também está ligada à placa 2,
que por sua vez está ligada às placas 14 e 16.

<pre>
Esquemático de comunicação dos Arduinos
    ______
   |      |
   |  PC  |
   |______|
           \    _____
            \  /     \
         ____\/   5   \_____
        /     \       /     \
       /  13   \_____/   9   \
       \       /\_   \       /
        \_____/   4   \_____/
        /     \   |   /     \
  _____/  12   \__|__/   8   \_____
 /     \       /  |  \       /     \
/  17   \_____/   3   \_____/  15   \
\       /     \   |   /     \       /
 \_____/  11   \__|__/   7   \_____/
 /     \       /  |  \       /     \
/  16__ \_____/   2   \_____/ _14   \
\      \/     \__/ \__/     \/      /
 \_____/\_10__/\_____/\__6__/\_____/
       \       /     \       /
        \_____/   1   \_____/
              \       /
               \_____/
</pre>

Para evitar que o ruído da rede elétrica influencie na leitura dos sensores
piezo-elétricos, é utilizado um circuito opto-isolador na placa 4. Assim,
a placa 4 tem um cabo denominado 'PC', que deve ser ligado ao computador,
e outro cabo denominado 'Tomada', que deve ser ligado em alguma fonte USB
que não apresente ruído, como um Power Bank USB ou um computador com a
bateria carregada e que não esteja ligado na tomada.

Cada sensor piezo-elétrico está ligado ao Arduino mais próximo.

No Arduino 2 estão ligadas as placas 1, 3, 6, 7, 10 e 11.

No Arduino 4 estão ligadas as placas 5, 8, 9, 12 e 13.

No Arduino 14 está ligada a placa 15.

No Arduino 16 está ligada a placa 17.

<pre>
 Esquemático de sensores por Arduino
                _____
               /     \
         _____/   5   \_____
        /     \   |   /     \
       /  13__ \__|__/ __9   \
       \      \/  |  \/      /
        \_____/\__4__/\_____/
        /     \/     \/     \
  _____/  12__/\_____/\__8   \_____
 /     \       /     \       /     \
/  17   \_____/   3   \_____/  15   \
\   |   /     \   |   /     \   |   /
 \__|__/  11__ \__|__/ __7   \__|__/
 /  |  \      \/  |  \/      /  |  \
/  16   \_____/\__2__/\_____/  14   \
\       /     \/  |  \/     \       /
 \_____/  10__/\__|__/\__6   \_____/
       \       /  |  \       /
        \_____/   1   \_____/
              \       /
               \_____/
</pre>
