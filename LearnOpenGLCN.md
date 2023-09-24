# LearnOpenGL 中文翻译版笔记

如果您发现侵犯了您的版权，请联系删除。

## 图形渲染管线

![图形渲染管线的抽象展示](https://learnopengl-cn.github.io/img/01/04/pipeline.png)  
*图形渲染管线的抽象展示*

## 数据格式

### 顶点缓存数据格式

顶点缓冲数据会被解析为下面这样子：

1. 位置数据被储存为32位（4字节）浮点值。
2. 每个位置包含3个这样的值。
3. 在这3个值之间没有空隙（或其他值）。这几个值在数组中紧密排列(Tightly Packed)。
4. 数据中第一个值在缓冲开始的位置。
![顶点缓存数据格式](https://learnopengl-cn.github.io/img/01/04/vertex_attribute_pointer.png)
*顶点缓存数据格式*

使用OpenGL注册顶点缓存数据的API：

```c
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);
```

各个参数解释如下：

1. 第一个参数指定我们要配置的顶点属性。还记得我们在顶点着色器中使用`layout(location = 0)`定义了`position`顶点属性的位置值`(Location)`吗？它可以把顶点属性的位置值设置为`0`。因为我们希望把数据传递到这一个顶点属性中，所以这里我们传入`0`。
2. 第二个参数指定顶点属性的大小。顶点属性是一个`vec3`，它由3个值组成，所以大小是3。
3. 第三个参数指定数据的类型，这里是`GL_FLOAT`(GLSL中`vec*`都是由浮点数值组成的)。
4. 下个参数定义我们是否希望数据被标准化(Normalize)。如果我们设置为`GL_TRUE`，所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间。我们把它设置为`GL_FALSE`。
5. 第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。由于下个组位置数据在3个`float`之后，我们把步长设置为`3 * sizeof(float)`。要注意的是由于我们知道这个数组是紧密排列的（在两个顶点属性之间没有空隙）我们也可以设置为`0`来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）。一旦我们有更多的顶点属性，我们就必须更小心地定义每个顶点属性之间的间隔，我们在后面会看到更多的例子（译注: 这个参数的意思简单说就是从这个属性第二次出现的地方到整个数组0位置之间有多少字节）。
6. 最后一个参数的类型是`void*`，所以需要我们进行这个奇怪的强制类型转换。它表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0。我们会在后面详细解释这个参数。

创建并绑定VAO的正确方法：

```c
GLuint VAO;
glGenVertexArrays(1, &VAO);
// 注意一定要绑定VAO
glBindVertexArray(VAO);
```

创建并绑定VBO的正确方法：

```c
// Vertex Buffer Object的句柄
GLuint VBO;
// 生成一个Buffer
glGenBuffers(1, &VBO);
// 注册这个Buffer到当前上下文
glBindBuffer(GL_ARRAY_BUFFER, VBO);
// 将数据传入Buffer。这将会把数据从内存复制到显存
// 第四个参数可选值：
// GL_STATIC_DRAW ：数据不会或几乎不会改变。
// GL_DYNAMIC_DRAW：数据会被改变很多。
// GL_STREAM_DRAW ：数据每次绘制时都会改变。
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```

创建并绑定EBO的正确方法：

```c
GLuint EBO;
glGenBuffers(1, &EBO);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
```

### 顶点数组对象（VAO）

顶点数组对象用于管理顶点缓冲对象（VBO）。回忆一下，顶点缓冲对象用于管理顶点数据，包括将其加载到显存、设置顶点属性并与着色器的位置值绑定等。

## 纹理

### 纹理环绕方式

设定纹理环绕方式。纹理环绕方式是指OpenGL如何处理超出标准坐标取值范围的纹理坐标。

| 环绕方式           | 描述                                                                                   |
| ------------------ | -------------------------------------------------------------------------------------- |
| GL_REPEAT          | 对纹理的默认行为。重复纹理图像。                                                       |
| GL_MIRRORED_REPEAT | 和GL_REPEAT一样，但每次重复图片是镜像放置的。                                          |
| GL_CLAMP_TO_EDGE   | 纹理坐标会被约束在0到1之间，超出的部分会重复纹理坐标的边缘，产生一种边缘被拉伸的效果。 |
| GL_CLAMP_TO_BORDER | 超出的坐标为用户指定的边缘颜色。                                                       |

![纹理环绕](https://learnopengl-cn.github.io/img/01/06/texture_wrapping.png)

如何设定纹理环绕方式：

```c
// 1
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
// 2
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
// 3
float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
```

### 纹理过滤

纹理坐标不依赖于分辨率，它可以是任意浮点值，所以OpenGL需要知道怎样将纹理像素映射到纹理坐标。这个映射的过程被称为纹理过滤。

GL_NEAREST（也叫邻近过滤，Nearest Neighbor Filtering）是OpenGL默认的纹理过滤方式。当设置为GL_NEAREST的时候，OpenGL会选择中心点最接近纹理坐标的那个像素。下图中你可以看到四个像素，加号代表纹理坐标。左上角那个纹理像素的中心距离纹理坐标最近，所以它会被选择为样本颜色：

![纹理过滤图解](https://learnopengl-cn.github.io/img/01/06/filter_nearest.png)

GL_LINEAR（也叫线性过滤，(Bi)linear Filtering）它会基于纹理坐标附近的纹理像素，计算出一个插值，近似出这些纹理像素之间的颜色。一个纹理像素的中心距离纹理坐标越近，那么这个纹理像素的颜色对最终的样本颜色的贡献越大。下图中你可以看到返回的颜色是邻近像素的混合色：

![纹理过滤图解](https://learnopengl-cn.github.io/img/01/06/filter_linear.png)

两种不同的纹理过滤方式的总体对比：

![两种纹理过滤方式对比](https://learnopengl-cn.github.io/img/01/06/texture_filtering.png)

### 多级渐远纹理

利用OpenGL自动创建多级渐远纹理：

![多级纹理图解](https://learnopengl-cn.github.io/img/01/06/mipmaps.png)

在切换多级渐远纹理时，会产生硬过渡，此时可以在两个相邻的多级渐远纹理之间启用纹理过滤。

| 过滤方式                    | 描述                                                                     |
| --------------------------- | ------------------------------------------------------------------------ |
| `GL_NEAREST_MIPMAP_NEAREST` | 使用最邻近的多级渐远纹理来匹配像素大小，并使用邻近插值进行纹理采样       |
| `GL_LINEAR_MIPMAP_NEAREST`  | 使用最邻近的多级渐远纹理级别，并使用线性插值进行采样                     |
| `GL_NEAREST_MIPMAP_LINEAR`  | 在两个最匹配像素大小的多级渐远纹理之间进行线性插值，使用邻近插值进行采样 |
| `GL_LINEAR_MIPMAP_LINEAR`   | 在两个邻近的多级渐远纹理之间使用线性插值，并使用线性插值进行采样         |

设置方法：

```c
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
```

*一个常见的错误是，将放大过滤的选项设置为多级渐远纹理过滤选项之一。这样没有任何效果，因为多级渐远纹理主要是使用在纹理被缩小的情况下的：纹理放大不会使用多级渐远纹理，为放大过滤设置多级渐远纹理的选项会产生一个`GL_INVALID_ENUM`错误代码。*

使用纹理对象的示例代码：

```c
// 创建纹理对象，并绑定
unsigned int texture;
glGenTextures(1, &texture);
glBindTexture(GL_TEXTURE_2D, texture);
// 设置纹理
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
// 自动生成多级渐远纹理
glGenerateMipmap(GL_TEXTURE_2D);
```

函数参数解释：

1. 第一个参数指定了纹理目标(Target)。设置为`GL_TEXTURE_2D`意味着会生成与当前绑定的纹理对象在同一个目标上的纹理（任何绑定到`GL_TEXTURE_1D`和`GL_TEXTURE_3D`的纹理不会受到影响）。
2. 第二个参数为纹理指定多级渐远纹理的级别，如果你希望单独手动设置每个多级渐远纹理的级别的话。这里我们填`0`，也就是基本级别。
3. 第三个参数告诉OpenGL我们希望把纹理储存为何种格式。我们的图像只有RGB值，因此我们也把纹理储存为RGB值。
4. 第四个和第五个参数设置最终的纹理的宽度和高度。我们之前加载图像的时候储存了它们，所以我们使用对应的变量。
5. 下个参数应该总是被设为0（历史遗留的问题）。
6. 第七第八个参数定义了源图的格式和数据类型。我们使用RGB值加载这个图像，并把它们储存为`char(byte)`数组，我们将会传入对应值。
最后一个参数是真正的图像数据。

结合图像加载库，以及纹理环绕方式等，正确加载纹理应该是这样：

```c
unsigned int texture;
glGenTextures(1, &texture);
glBindTexture(GL_TEXTURE_2D, texture);
// 为当前绑定的纹理对象设置环绕、过滤方式
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// 加载并生成纹理
int width, height, nrChannels;
unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
if (data)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}
else
{
    std::cout << "Failed to load texture" << std::endl;
}
stbi_image_free(data);
```

使用glUniform1i，我们可以给纹理采样器分配一个位置值，这样的话我们能够在一个片段着色器中设置多个纹理。一个纹理的位置值通常称为一个纹理单元(Texture Unit)。一个纹理的默认纹理单元是0，它是默认的激活纹理单元。

在OpenGL中使用纹理单元的方法：

```c
glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
glBindTexture(GL_TEXTURE_2D, texture);
```

*OpenGL至少保证有16个纹理单元供你使用，也就是说你可以激活从`GL_TEXTURE0`到`GL_TEXTRUE15`。它们都是按顺序定义的，所以我们也可以通过`GL_TEXTURE0 + 8`的方式获得`GL_TEXTURE8`，这在当我们需要循环一些纹理单元的时候会很有用。*
