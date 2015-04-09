#include "FilePathManager.h"
#include "qpushbutton.h"
#include "qboxlayout.h"

QString FilePathManager::value(const QtProperty *property) const
{
	if (!theValues.contains(property)) return "";
	return theValues[property].value;
}

void FilePathManager::setValue(QtProperty *property, const QString &value)
{
	if (!theValues.contains(property)) return;
	Data data = theValues[property];
	if (data.value == value) return;
	data.value = value;
	theValues[property] = data;
	emit propertyChanged(property);
	emit valueChanged(property, data.value);
}

class FileEdit : public QWidget
{
public :
	FileEdit(QWidget* parent = nullptr)
		: QWidget(parent)
	{
		_lineedit = new QLineEdit(this);
		_browsebtn = new QPushButton(this);
		QBoxLayout* layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
		layout->addWidget(_lineedit);
		layout->addWidget(_browsebtn);
		this->setLayout(layout);
	}

private:
	QLineEdit* _lineedit;
	QPushButton* _browsebtn;
};

QWidget * FileEditFactory::createEditor(FilePathManager *manager, QtProperty *property, QWidget *parent)
{
	FileEdit* edit = new FileEdit;

	auto it = createdEditors.find(property);
	if (it == createdEditors.end())
		it = createdEditors.insert(property, QList<FileEdit *>());
	it.value().append(edit);
	editorToProperty.insert(edit, property);
}
