// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

#include "CoreLib/Database/Database.h"

namespace Ui { class LibraryView; }

class LibrarySqlModel : public QSqlQueryModel
{
    Q_OBJECT

protected:
    QSize m_viewIconSize;
    int	  m_fontSize;
    bool  m_isIconView;

public:
    LibrarySqlModel(QObject *parent = 0);
    ~LibrarySqlModel();

    QVariant data(const QModelIndex &item, int role) const;

    void SetIconView(bool isIconView)
    {
        if (isIconView)
        {
            m_isIconView = true;
            m_viewIconSize.setWidth(100);
            m_viewIconSize.setHeight(100);
            m_fontSize = 24;
        }
        else
        {
            m_isIconView = false;
            m_viewIconSize.setWidth(24);
            m_viewIconSize.setHeight(24);
            m_fontSize = 10;
        }
    }

    bool IsIconView() const				{ return m_isIconView; }
};

class FolderItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    FolderItemDelegate(QObject* pParent = nullptr);
    ~FolderItemDelegate();

    virtual QWidget* createEditor(QWidget* pParent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void	 paint(QPainter* pPainter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual QSize	 sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void	 setEditorData(QWidget* pEditor, const QModelIndex & index ) const;
    virtual void	 setModelData(QWidget* pEditor, QAbstractItemModel* pModel, const QModelIndex &index) const;

private:
    Q_DISABLE_COPY(FolderItemDelegate)
};

class LibraryView : public QWidget
{
    Q_OBJECT

private:
    Ui::LibraryView* ui;
    QAction*		m_pAction;
    QIcon			m_icons[3][2];
    LibrarySqlModel m_designsModel;

    static QString settingsPath_viewIcons;
    static QString settingsPath_splitter;
    static QString settingsPath_columns;
    static QString settingsPath_columnSort;
    static QString folderIdStr;

    QTreeWidgetItem* m_pTreeEditItem;
    QString			 m_treeEditItemText;

    enum IconType
    {
        Folder,
        ShowAll,
        RecycleBin
    };

    qint64 AddFolder(QTreeWidgetItem* pParent, qint64 folderId, const QString& name, IconType iconType, DbPreparedQuery& countDesignsQuery); 
    void SetColumns();

    QAction* CreateSortMenuItem(const QString& text, int data, bool isChecked);
    void CloseTreeEdit();
    void OpenTreeEdit();
    void ContextMenuRequested(const QPoint &pos, const QWidget* pWidget);

public:
    explicit LibraryView(QAction* pAction, QWidget* pParent = 0);
    ~LibraryView();

private slots:
    void OnColumnResized(int column, int oldSize, int newSize);
    void OnColumnSortChanged(int column, Qt::SortOrder order);
    void OnDeleteDesignListView(bool checked);
    void OnRenameDesignListView(bool checked);
    void OnDeleteDesignIconView(bool checked);
    void OnRenameDesignIconView(bool checked);
    void OnEmptyRecycleBin(bool checked);
    void OnDeleteFolder(bool checked);
    void OnRenameFolder(bool checked);
    void OnNewFolder(bool checked);

private slots:
    void on_closeButton_clicked();
    void on_treeWidget_itemSelectionChanged();
    void on_iconViewButton_clicked(bool checked);
    void on_listViewButton_clicked(bool checked);
    void on_splitter_splitterMoved(int pos, int index);
    void on_lineEdit_textChanged(const QString &arg1);
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void on_iconView_customContextMenuRequested(const QPoint &pos);
    void on_listView_customContextMenuRequested(const QPoint &pos);
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_treeWidget_itemChanged(QTreeWidgetItem* pItem, int column);
};

