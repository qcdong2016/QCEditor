#ifndef _FILE_PATH_MANAGER_H_
#define _FILE_PATH_MANAGER_H_

#include "qtpropertybrowser.h"



class FilePathManager : public QtAbstractPropertyManager
{
	Q_OBJECT

public:
	QString value(const QtProperty *property) const;
	QString filter(const QtProperty *property) const;
public slots:
	void setValue(QtProperty *, const QString &);
	void setFilter(QtProperty *, const QString &);
signals:
	void valueChanged(QtProperty *, const QString &);
	void filterChanged(QtProperty *, const QString &);
protected:
	QString valueText(const QtProperty *property) const
	{
		return value(property);
	}
	void initializeProperty(QtProperty *property)
	{
		theValues[property] = Data();
	}
	void uninitializeProperty(QtProperty *property)
	{
		theValues.remove(property);
	}
private:
	struct Data {
		QString value;
		QString filter;
	};
	QMap<const QtProperty *, Data> theValues;
};

class FileEditFactory : public QtAbstractEditorFactory<FilePathManager>
{
	Q_OBJECT

private slots:
	void slotPropertyChanged(QtProperty *property,
		const QString &value);
	void slotFilterChanged(QtProperty *property,
		const QString &filter);
	void slotSetValue(const QString &value);
	void slotEditorDestroyed(QObject *object);
private:
	QMap<QtProperty *, QList<FileEdit *> > createdEditors;
	QMap<FileEdit *, QtProperty *> editorToProperty;
};


#endif//_FILE_PATH_MANAGER_H_