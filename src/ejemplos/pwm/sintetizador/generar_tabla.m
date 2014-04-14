% Programa de Octave/Matlab para generar muestras de 8 bits de una señal senoidal
% y almacenarlas en un archivo de texto.

% cantidad de puntos a muestrear
N = 32;

% generamos un vector de N valores secuenciales de "tiempo" entre 0 y 2pi
% para graficar un período completo del seno
t = linspace(0, 2*pi, N);

% generamos el vector con los valores del seno para cada punto de t
s = sin(t);

% adaptamos el rango de amplitud del seno de [-1; 1] a [0; 255] para que entre en un byte
% y rendondeamos al entero más cercano
s2 = s - min(s);
s2 = round(s2/max(s2) * 255);

% graficamos las señales que acabamos de generar en función del tiempo
plot(t, s*128, 'b', t, s2, '+r');

% guardamos el vector de señal en un archivo de texto separado por comas
csvwrite('sin_table.txt', s2);
