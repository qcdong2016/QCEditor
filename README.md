![preview][]

#QCEditor[该项目被列为OSCHINA 2015 top100第96位](https://www.oschina.net/news/69808/2015-annual-ranking-top-100-new-open-source-software)

**QCEditor**是一个简单的cocos2d-x编辑器。 Qt和Cocos-x 都能跨平台，理论上是能够跨平台的。名字是 Qt 和 Cocos2d-x 的首字母缩写。写这个编辑器的目的是为了实现一个能摆放ui控件位置的功能，我们在游戏开发过程中通常也只会用到这个功能。

**QCEditor**的渲染使用的是cocos2d-x本身的渲染，所以说在编辑器里面编辑的效果和游戏中的效果会一模一样，在编辑器里面添加的控件代码，不用再到项目里面实现一次，只需要拷贝过去就能用。添加新控件只需注册属性就好，无需再添加新解析代码。（目前解析部分能支持大部分常用类型解析）。

##编译过程
* VS2013 + Qt 5.4.1 + QtCreator
* 先用QtCreator编译qteditor\qtpropertybrowser\buildlib\buildlib.pro
* 然后打开工程修改Qt版本为你本机版本
* 编译运行

##如何使用
* 拷贝libqc文件夹下的文件到你的项目
* 包含Serializer.h 调用Serializer::read（整个库只有这一个API）

##功能添加
添加新的属性和控件非常方便，只需要按照下面格式添加，将会自动与Node绑定，并且自动添加到存储功能中。通过这种方式，能够编辑cocos2d-x里所有可视化组件，甚至其他一些组件。

```C++
	StartGroup(Node, defaultNodeCtor);
	ATTR("Local Z Order", &Node::getLocalZOrder, &Node::setLocalZOrder, int, 0);
	ATTR("Global Z Order", &Node::getGlobalZOrder, &Node::setGlobalZOrder, float, 0.0f);
	ATTR("Visible", &Node::isVisible, &Node::setVisible, bool, true);
	ATTRSTEP("Scale X", &Node::getScaleX, &Node::setScaleX, float, 1.0, 0.1);
	ATTRSTEP("Scale Y", &Node::getScaleY, &Node::setScaleY, float, 1.0, 0.1);
	ATTR("Rotation", &Node::getRotation, &Node::setRotation, float, 0);
	ATTR("Position", &Node::getPosition, &Node::setPosition, Vec2, Vec2(0, 0));
	ATTR("Tag", &Node::getTag, &Node::setTag, int, 0);
	ATTRMixed("Name", &Node::getName, &Node::setName, std::string, std::string());
	ATTRMMS("Anchor Pos", &Node::getAnchorPoint, &Node::setAnchorPoint, Vec2, Vec2(0, 0), Vec2(0, 0), Vec2(1, 1), Vec2(0.1, 0.1));
	EndGroup();
```

添加LabelTTF到编辑器代码如下：
```C++
	StartGroup(LabelTTF, DefaultValue::defaultLabelTTF);
	Require(Node);
	ATTR_NO_DEFAULT("Font File", &LabelTTF::getFontName, &LabelTTF::setFontName, string, Variant::TString);
	ATTR("Font Size", &LabelTTF::getFontSize, &LabelTTF::setFontSize, float, 20);
	ATTR("Dimensions", &LabelTTF::getDimensions, &LabelTTF::setDimensions, Size, Size(0,0));
	ATTR("String", &LabelTTF::getString, &LabelTTF::setString, string, string());
	EndGroup();
```

xml文件格式如下：

```XML
<?xml version='1.0' encoding='utf-8' ?>
<Root>
	<UI>
		<Node>
			<Property name="Position" value="340 190.5"/>
			<Property name="Tag" value="-1"/>
			<LabelTTF>
				<Property name="String" value="wwwd"/>
				<Property name="Font File" value="Marker Felt.ttf"/>
				
				<ParticleSystemQuad>
					<Property name="Duration" value="-1"/>
					<Property name="Life" value="4"/>
					<Property name="Life Var" value="1"/>
					<Property name="Angle" value="90"/>
					<Property name="Angle Var" value="360"/>
					<Property name="Start Size" value="30"/>
					<Property name="Start Size Var" value="10"/>
					<Property name="End Size" value="30"/>
					<Property name="Start Color" value="0.5 0.5 0.5 1"/>
					<Property name="Start Color Var" value="0.5 0.5 0.5 0"/>
					<Property name="End Color" value="0 0 0 1"/>
					<Property name="End Color Var" value="0 0 0 0"/>
					<Property name="Emission Rate" value="62.5"/>
					<Property name="Total Particles" value="250"/>
					<Node>
						<Property name="Position" value="-67 -369"/>
						<Property name="Tag" value="-1"/>
					</Node>
				</ParticleSystemQuad>
			</LabelTTF>
		</Node>
	</UI>
</Root>

```

在编辑器里面看到的是这样：

![attr][]


##项目地址

* oschina  [http://git.oschina.net/qcdong/QCEditor](http://git.oschina.net/qcdong/QCEditor)
* github   [https://github.com/CaptainCN/QCEditor](https://github.com/CaptainCN/QCEditor)


##其他
个人力量有限，如果有小伙伴一起来搞就好啦，欢迎提供代码。QQ(526738316) Email(qcdong@yeah.net)

##License
Licensed under the MIT license, see [LICENSE](LICENSE) for details.

###说明
	该项目提交了cocos2d-x 3.4创建的工程中的cocos文件夹（解压约380M），仓库大小大约120M，所以拉项目的时候比较慢。但项目本身代码并不多。


[preview]: images/preview.jpg
[attr]: images/attr.png