#include "FilePathManager.h"

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

