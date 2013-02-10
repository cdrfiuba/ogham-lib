% Programa de Octave/Matlab para generar muestras de 8 bits de una señal senoidal
% y alamcenarlas en un archivo de texto.

% cantidad de puntos a muestrear
N = 256;

% generamos un vector de valores lineales de "tiempo" entre 0 y 2pi
t = linspace(0, 2*pi, N);

% generamos el vector con los valores del seno para cada punto de t
s = sin(t);

% adaptamos el rango de amplitud del seño de [-1; 1] a [0; 255] para que entre en un byte
s2 = s - min(s);
s2 = round(s2/max(s2) * 255);

% graficamos las señales que acabamos de generar en funcion del tiempo
plot(t, s*128, 'b', t, s2, '+r');

% guardamos el vector de señal en un archivo de texto separado por comas
csvwrite('sin_table.txt', s2);
