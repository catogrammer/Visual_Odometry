# Essential Matrix

В компьютерном зрении , essential matrix является матрицей размера  $3 \times 3$ , $E$ с некоторыми дополнительными свойствами, описанными ниже, которые связывают соответствующие точки в стереоизображениях, предполагая, что камеры удовлетворяют модели камеры обскуры .

***Essential matrix*** можно рассматривать как предшественника ***fundamental matrix***. Обе матрицы могут использоваться для установления ограничений между совпадающими точками изображения, но существенная матрица может использоваться только в отношении калиброванных камер, поскольку параметры внутренней камеры должны быть известны для достижения нормализации. Если, однако, камеры откалиброваны, необходимая матрица может быть полезна для определения как относительного положения, так и ориентации между камерами и трехмерным положением соответствующих точек изображения.
$$
E = R[t]_x ;
$$
где $[t]_x$ матричное  представление поперечного произведения  $_ct$
$\begin{bmatrix}
R & 0 \\
0 & 1 \\
\end{bmatrix}$ а  также $\begin{bmatrix}
I & t \\
0 & 1 \\
\end{bmatrix}$
R - матрица поворта $3\times 3​$
t - трёхмерный  вектор сдвига

## Properties
* rank E = 2

## Exploration

1. Изначально мы имеем два изображения с камеры  $I_1, I_2$, где первое изображние это избражение с позиции $T_1$, а второе с позиции $T_2$
2. Вычисляем $\acute{\tilde{p}}E\tilde{p} = 0$, где $\acute{\tilde{p}}$ – местоположение объекта на одном изображении (например, $I_{k}$), а  $\tilde{p}$ – местоположение его соответствующей особенности в другом изображении (например, $I_{k-1}$),  $\acute{\tilde{p}}$ и $\tilde{p}$  нормированные координаты изображения. Для простоты в следующих разделах будут использоваться нормированные координаты в виде $\acute{\tilde{p}} = [ \tilde{u}, \tilde{v}, 1]$

   * Нормализация координат [перспективная камера] (какую мы используем или пишем сразу для нескольких?)
     $Cartesain (Euclidian) \to homogenous$

     $P=(x,y) \quad P\in\mathbb R^2 \quad \to \quad  P=(x,y,1)  \quad P\in\mathbb P^2$

     $homogenous \to Cartesain (Euclidian)​$

     $\tilde{P}=(\tilde{x}, \tilde{y}, \tilde{z})  \qquad P=(x,y)$ 	где  $\tilde{z}$ - коэффициент масштабирования

$$
x = \frac{\tilde{x}}{\tilde{z}} \qquad y = \frac{\tilde{y}}{\tilde{z}}
$$

$$
\lambda \begin{bmatrix} u \\ v \\ 1\end{bmatrix} = KX = \begin{bmatrix} a_u & 0 & u_0 \\ 0 & a_v & v_0 \\ 0 & 0 & 1 \end{bmatrix}\begin{bmatrix} x \\ y \\ z\end{bmatrix}
$$
​		where $\lambda$ - deepth factor
​		$a_u$ and $a_v$ - focal lengths
​	 	$u_0$ and $v_0$ - image coordinates of the projection center.

2. Две позиции камеры при смежных моментах времени k – 1 и k связаны преобразованием закрепленного тела $T_{k,k-1} \in R^{4\times4}$ следующего вида $T_{k,k-1} = \begin{bmatrix} R_{k,k-1} & t_{k,k-1} \\0 & 1 \\\end{bmatrix}$

  $$E_k \equiv \hat{t}_kR_k\;$$		где		$\;t_k=[t_x, t_y, t_z]$
  $$\hat{t}_k= \begin{bmatrix} 0 & -t_z & t_y \\ t_z & 0 & -t_x \\ -t_y & t_x & 0\end{bmatrix}$$
Символ $\equiv$ используется для обозначения того, что эквивалентность действительна до мультипликативного скаляра. (Существенную матрицу можно вычислить из соответствия двух-двухзначных признаков, и вращение, и преобразование можно непосредственно извлечь из $E$)

5. $t$ - трехмерный вектор сдвига (вектор между $C_{k-1}$ и $C_k$)

6. Далее, что из себя представляет матрица поворота (не знаю что мы туда  записываем)
​	Основным свойством оценки движения на основе 2-D-to-2-D является эпиполярное ограничение, которое определяет линию, на которой соответствующая точка объекта $\acute{\tilde{p}}$ из $\tilde{p}$ лежит на другом изображении (рис. 4). Это ограничение можно сформулировать с помощью $\acute{\tilde{p}}E\tilde{p} = 0$, где $\acute{\tilde{p}}$ – местоположение объекта на одном изображении (например, ), а  $\tilde{p}$ – местоположение его соответствующей особенности в другом изображении (например, $I_{k-1}$),  $\acute{\tilde{p}}$ и $\tilde{p}$  нормированные координаты изображения. Для простоты в следующих разделах будут использоваться нормированные координаты в виде $\acute{\tilde{p}} = [ \tilde{u}, \tilde{v}, 1]$ (См. Раздел «Модель перспективной камеры»). Однако очень похожие уравнения могут быть также получены для нормированных координат на единичной сфере (см. Раздел «Сферическая модель»).

# Step Calculation


2. Normilized coordinats (?)
$$
{\begin{pmatrix}y_{1}\\y_{2}\end{pmatrix}}={\frac  {1}{x_{3}}}{\begin{pmatrix}x_{1}\\x_{2}\end{pmatrix}} \qquad {\begin{pmatrix}y'_{1}\\y'_{2}\end{pmatrix}}={\frac  {1}{x'_{3}}}{\begin{pmatrix}x'_{1}\\x'_{2}\end{pmatrix}} \\
{\begin{pmatrix}y_{1}\\y_{2}\\1\end{pmatrix}}={\frac  {1}{x_{3}}}{\begin{pmatrix}x_{1}\\x_{2}\\x_{{3}}\end{pmatrix}} \qquad {\begin{pmatrix}y'_{1}\\y'_{2}\\1\end{pmatrix}}={\frac  {1}{x'_{3}}}{\begin{pmatrix}x'_{1}\\x'_{2}\\x'_{{3}}\end{pmatrix}}
$$
which also can be written more compactly as
$$
{\mathbf  {y}}={\frac  {1}{x_{{3}}}}\,{\tilde  {{\mathbf  {x}}}} \qquad
{\mathbf  {y}}'={\frac  {1}{x'_{{3}}}}\,{\tilde  {{\mathbf  {x}}}}'
$$


$$
\acute{\tilde{p}}E\tilde{p} = 0
$$
3.
   где $\acute{\tilde{p}}$ – местоположение объекта на одном изображении (например, ), а  $\tilde{p}$ – местоположение его соответствующей особенности в другом изображении (например, $I_{k-1}$),  $\acute{\tilde{p}}$ и $\tilde{p}$  нормированные координаты изображения

# Link for use mathematical library of linear algeba

**Comparasion**
https://en.wikipedia.org/wiki/Comparison_of_linear_algebra_libraries
https://scicomp.stackexchange.com/questions/351/recommendations-for-a-usable-fast-c-matrix-library
http://seldon.sourceforge.net/
https://math.nist.gov/lapack++/
https://www.boost.org/doc/libs/1_66_0/libs/numeric/ublas/doc/index.html
https://en.wikipedia.org/wiki/Armadillo_(C%2B%2B_library)
https://en.wikipedia.org/wiki/Dlib
