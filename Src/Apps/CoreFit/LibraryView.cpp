// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"
#include "LibraryView.h"
#include "ui_LibraryView.h"
#include <CoreLib/Database/MakeKeywordQuery.h>
#include <CoreLib/ErrCode.h>
#include "CoreFit.h"

QString LibraryView::settingsPath_viewIcons = QStringLiteral("library/viewicons");
QString LibraryView::settingsPath_splitter = QStringLiteral("library/splitter");
QString LibraryView::settingsPath_columns = QStringLiteral("library/columns/");
QString LibraryView::settingsPath_columnSort = QStringLiteral("library/columns/order");
QString LibraryView::folderIdStr = QStringLiteral(":folderId");

LibraryView::LibraryView(QAction* pAction, QWidget* pParent)
    : QWidget(pParent)
    , ui(new Ui::LibraryView)
    , m_pAction(pAction)
    , m_pTreeEditItem(nullptr)
{
    ui->setupUi(this);
    ui->treeWidget->setItemDelegate(new FolderItemDelegate);

    QByteArray splitterState = pSettings->value(settingsPath_splitter).toByteArray();
    ui->splitter->restoreState(splitterState);

    ui->iconView->setModel(&m_designsModel);
    QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(&m_designsModel);
    ui->listView->setModel(proxyModel);

    if (pSettings->value(settingsPath_viewIcons, true).toBool())
    {
        ui->listView->hide();
        ui->listViewButton->setChecked(false);

        ui->iconView->show();
        ui->iconViewButton->setChecked(true);

        m_designsModel.SetIconView(true);
    }
    else
    {
        ui->iconView->hide();
        ui->iconViewButton->setChecked(false);

        ui->listView->show();
        ui->listViewButton->setChecked(true);

        m_designsModel.SetIconView(false);
    }

    //QPainter painter;
    //painter.begin(&m_transparentThumbnail);
    //painter.drawPixmap(pix->rect(),*m_image);
    //painter.end();

    m_icons[Folder][0].addFile(QStringLiteral(":/Studio/Resources/Icons/folder_empty.ico"), QSize(), QIcon::Normal, QIcon::Off);
    m_icons[Folder][1].addFile(QStringLiteral(":/Studio/Resources/Icons/folder_full.ico"), QSize(), QIcon::Normal, QIcon::Off);

    m_icons[ShowAll][0].addFile(QStringLiteral(":/Studio/Resources/Icons/show_all_empty.ico"), QSize(), QIcon::Normal, QIcon::Off);
    m_icons[ShowAll][1].addFile(QStringLiteral(":/Studio/Resources/Icons/show_all_full.ico"), QSize(), QIcon::Normal, QIcon::Off);

    m_icons[RecycleBin][0].addFile(QStringLiteral(":/Studio/Resources/Icons/recycle_bin_empty.ico"), QSize(), QIcon::Normal, QIcon::Off);
    m_icons[RecycleBin][1].addFile(QStringLiteral(":/Studio/Resources/Icons/recycle_bin_full.ico"), QSize(), QIcon::Normal, QIcon::Off);

    QTreeWidgetItem* pShowAllFolder = new QTreeWidgetItem(ui->treeWidget);
//    pShowAllFolder->setData(0, Qt::UserRole, (qint64)LibRootFolders::ShowAllFiles);

    //DbPreparedQuery countDesignsQuery(QStringLiteral("SELECT COUNT(*) FROM designs WHERE folderId=:folderId"), _WHERE_, "count designs in a folder");

    //qint64 totalDesigns = LibraryView::AddFolder(nullptr, (qint64)LibRootFolders::Fonts, tr("Fonts"), Folder, countDesignsQuery);
    //totalDesigns += LibraryView::AddFolder(nullptr, (qint64)LibRootFolders::Patterns, tr("Patterns"), Folder, countDesignsQuery);
    //totalDesigns += LibraryView::AddFolder(nullptr, (qint64)LibRootFolders::Designs, tr("Designs"), Folder, countDesignsQuery);
    //totalDesigns += LibraryView::AddFolder(nullptr, (qint64)LibRootFolders::MyLibrary, tr("My Library"), Folder, countDesignsQuery);
    //totalDesigns += LibraryView::AddFolder(nullptr, (qint64)LibRootFolders::RecycleBin, tr("Recycle Bin"), RecycleBin, countDesignsQuery);

    //if (totalDesigns)
    //{
    //    QLocale locale;
    //    pShowAllFolder->setText(0, tr("Show All Designs") % QStringLiteral(" (%L1)").arg(totalDesigns));
    //}
    //else
    //{
    //    pShowAllFolder->setText(0, tr("Show All Designs"));
    //}

    pShowAllFolder->setIcon(0, m_icons[ShowAll][/*totalDesigns > 0 ?*/ 1 /*: 0*/]);
    pShowAllFolder->setFlags(pShowAllFolder->flags() | Qt::ItemNeverHasChildren);

    connect(ui->listView->header(), SIGNAL(sectionResized(int, int, int)), this, SLOT(OnColumnResized(int, int, int)));
    connect(ui->listView->header(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), this, SLOT(OnColumnSortChanged(int, Qt::SortOrder)));
}

LibraryView::~LibraryView()
{
    delete ui;
}

qint64 LibraryView::AddFolder(QTreeWidgetItem* pParent, qint64 folderId, const QString& name, IconType iconType, DbPreparedQuery& countDesignsQuery)
{
    QTreeWidgetItem* pFolder = pParent ? new QTreeWidgetItem(pParent) : new QTreeWidgetItem(ui->treeWidget); //  deepcode ignore CppMemoryLeak: This is how Qt works
    pFolder->setData(0, Qt::UserRole, folderId);
    if (pParent)
        pFolder->setFlags(pFolder->flags() | Qt::ItemIsEditable);

    countDesignsQuery.bindValue(folderIdStr, folderId);
    countDesignsQuery.exec(_WHERE_);

    quint64 subTotalDesigns = countDesignsQuery.first() ? countDesignsQuery.value(0).toLongLong() : 0;
    bool hasChildren = subTotalDesigns > 0;

    static QString queryStr = QStringLiteral("SELECT folderId,name FROM folders WHERE parentId=%1 ORDER BY name COLLATE NOCASE");

    DbQuery subFoldersQuery;
    if (subFoldersQuery.exec(queryStr.arg(folderId), _WHERE_, "subFoldersQuery"))
    {
        while (subFoldersQuery.next())
        {
            hasChildren = true;

            qint64 subFolderId = subFoldersQuery.value(0).toLongLong();
            QString subFolderName = subFoldersQuery.value(1).toString();

            subTotalDesigns += AddFolder(pFolder, subFolderId, subFolderName, Folder, countDesignsQuery);
        }
    }

    if (subTotalDesigns)
    {
        static QString suffixStr = QStringLiteral(" (%L1)");
        pFolder->setText(0, name % suffixStr.arg(subTotalDesigns));
    }
    else
    {
        pFolder->setText(0, name);
    }

    pFolder->setData(0, Qt::UserRole + 1, name);
    pFolder->setData(0, Qt::UserRole + 2, subTotalDesigns);

    pFolder->setIcon(0, m_icons[iconType][hasChildren ? 1 : 0]);
    return subTotalDesigns;
}

void LibraryView::SetColumns()
{
    static bool columnsSet = false;

    if (!columnsSet)
    {
        columnsSet = true;

        //ui->listView->setColumnWidth(0, pSettings->value(settingsPath_columns % QString::number(0), 200).toInt());
        ui->listView->setColumnWidth(1, pSettings->value(settingsPath_columns % QString::number(1), 90).toInt());
        ui->listView->setColumnWidth(2, pSettings->value(settingsPath_columns % QString::number(2), 150).toInt());
        ui->listView->setColumnWidth(3, pSettings->value(settingsPath_columns % QString::number(3), 130).toInt());
        ui->listView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
        //ui->listView->header()->setSectionResizeMode(2, QHeaderView::Stretch);

        int orderColumn = pSettings->value(settingsPath_columnSort, 1).toInt();
        if (orderColumn > 0)
            ui->listView->header()->setSortIndicator(orderColumn - 1, Qt::AscendingOrder);
        else
            ui->listView->header()->setSortIndicator(-orderColumn - 1, Qt::DescendingOrder);

        ui->listView->setColumnHidden(4, true); //-V112
        ui->listView->setColumnHidden(5, true);

        m_designsModel.setHeaderData(0, Qt::Horizontal, tr("Name"));
        m_designsModel.setHeaderData(1, Qt::Horizontal, tr("Type"));
        m_designsModel.setHeaderData(2, Qt::Horizontal, tr("Artist"));
        m_designsModel.setHeaderData(3, Qt::Horizontal, tr("Modified Date"));
        //m_designsModel.setHeaderData(3, Qt::Horizontal, tr("Size"));
    }
}

void LibraryView::on_closeButton_clicked()
{
    m_pAction->setChecked(false);
    hide();
}

void LibraryView::CloseTreeEdit()
{
    if (m_pTreeEditItem)
    {
        ui->treeWidget->closePersistentEditor(m_pTreeEditItem, 0);
    }
}

void LibraryView::OpenTreeEdit()
{
    if (m_pTreeEditItem)
    {
        m_treeEditItemText = m_pTreeEditItem->text(0);
        //QString editableText = m_treeEditItemText;
        ui->treeWidget->openPersistentEditor(m_pTreeEditItem, 0);
    }
}

void LibraryView::on_treeWidget_itemChanged(QTreeWidgetItem* /*pItem*/, int /*column*/)
{
}

void LibraryView::on_treeWidget_itemDoubleClicked(QTreeWidgetItem* /*item*/, int /*column*/)
{
    //ui->treeWidget->editItem(item, column);
    //CloseTreeEdit();
    //m_pTreeEditItem = item;
    //OpenTreeEdit();
}

void LibraryView::on_treeWidget_itemSelectionChanged()
{
    CloseTreeEdit();

    QList<QTreeWidgetItem*> selectedItems = ui->treeWidget->selectedItems();
    if (selectedItems.size())
    {
        try
        {
            //qint64 folderId = selectedItems[0]->data(0, Qt::UserRole).toLongLong();

            //if (folderId == (qint64)LibRootFolders::ShowAllFiles)
            //{
            //    static QString queryStr(QStringLiteral("SELECT name,type,artist,modifiedDate,thumbnail,designId FROM designs ORDER BY name COLLATE NOCASE"));

            //    LibExecQuery designsQuery(queryStr, _WHERE_, "get all designs");
            //    m_designsModel.setQuery(designsQuery);
            //}
            //else
            //{
            //    static QString queryStr(QStringLiteral("SELECT name,type,artist,modifiedDate,thumbnail,designId FROM designs WHERE folderId=:folderId ORDER BY name COLLATE NOCASE"));

            //    DbPreparedQuery designsQuery(queryStr, _WHERE_, "get folder designs");
            //    designsQuery.bindValue(folderIdStr, folderId);
            //    designsQuery.exec(_WHERE_);
            //    m_designsModel.setQuery(designsQuery);
            //}

            SetColumns();
        }
        catch (const DbException& e)
        {
            e.Log(nullptr);
        }
    }
    else
    {
        m_designsModel.clear();
    }
}

void LibraryView::on_iconViewButton_clicked(bool checked)
{
    if (checked)
    {
        ui->listView->hide();
        ui->listViewButton->setChecked(false);

        ui->iconView->show();
        m_designsModel.SetIconView(true);
        pSettings->setValue(settingsPath_viewIcons, true);
        pSettings->setValue(settingsPath_splitter, ui->splitter->saveState());
    }
}

void LibraryView::on_listViewButton_clicked(bool checked)
{
    if (checked)
    {
        ui->iconView->hide();
        ui->iconViewButton->setChecked(false);

        ui->listView->show();
        m_designsModel.SetIconView(false);
        pSettings->setValue(settingsPath_viewIcons, false);
        pSettings->setValue(settingsPath_splitter, ui->splitter->saveState());
    }
}

void LibraryView::on_splitter_splitterMoved(int /*pos*/, int /*index*/)
{
    pSettings->setValue(settingsPath_splitter, ui->splitter->saveState());
}

void LibraryView::on_lineEdit_textChanged(const QString& searchStr)
{
    static QString queryPrefix(QStringLiteral("SELECT name,type,artist,modifiedDate,thumbnail,designId FROM designs WHERE "));
    static QString querySuffix(QStringLiteral("ORDER BY name COLLATE NOCASE"));

    QString queryStr = MakeKeywordQuery(searchStr);

    DbQuery searchQuery;
    if (searchQuery.exec(queryPrefix % queryStr % querySuffix, _WHERE_))
    {
        m_designsModel.setQuery(searchQuery);
        SetColumns();
    }
}

void LibraryView::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    QList<QTreeWidgetItem*> selectedItems = ui->treeWidget->selectedItems();
    if (selectedItems.size())
    {
        QTreeWidgetItem* pFolder = selectedItems[0];
        if (pFolder)
        {
            qint64 folderId = pFolder->data(0, Qt::UserRole).toLongLong();
            if (folderId != 0)//(qint64)LibRootFolders::ShowAllFiles)
            {
                QMenu menu(this);

                menu.addAction(tr("New Folder"), this, SLOT(OnNewFolder(bool)));

                //if (folderId == (qint64)LibRootFolders::RecycleBin)
                //{
                //    menu.addAction(tr("Empty Recycle Bin"), this, SLOT(OnEmptyRecycleBin(bool)));
                //}
                //else if (folderId > (qint64)LibRootFolders::_MaxRoot_)
                //{
                //    menu.addAction(tr("Delete Folder"), this, SLOT(OnDeleteFolder(bool)));
                //    menu.addAction(tr("Rename Folder"), this, SLOT(OnRenameFolder(bool)));
                //}

                menu.exec(ui->treeWidget->mapToGlobal(pos));
            }
        }
    }
}

void LibraryView::OnEmptyRecycleBin(bool)
{
    QList<QTreeWidgetItem*> selectedItems = ui->treeWidget->selectedItems();
    if (selectedItems.size())
    {
        QMessageBox msgBox;
        msgBox.setText(QStringLiteral("Empty Recycle Bin"));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

void LibraryView::OnDeleteFolder(bool)
{
    QList<QTreeWidgetItem*> selectedItems = ui->treeWidget->selectedItems();
    if (selectedItems.size())
    {
        QTreeWidgetItem* pFolder = selectedItems[0];

        QMessageBox msgBox;
        msgBox.setText(QStringLiteral("Delete Folder (%1)").arg(pFolder->text(0)));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

void LibraryView::OnRenameFolder(bool)
{
    QList<QTreeWidgetItem*> selectedItems = ui->treeWidget->selectedItems();
    if (selectedItems.size())
    {
        QTreeWidgetItem* pFolder = selectedItems[0];

        QMessageBox msgBox;
        msgBox.setText(QStringLiteral("Rename Folder (%1)").arg(pFolder->text(0)));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

void LibraryView::OnNewFolder(bool)
{
    QList<QTreeWidgetItem*> selectedItems = ui->treeWidget->selectedItems();
    if (selectedItems.size())
    {
        QTreeWidgetItem* pFolder = selectedItems[0];

        QMessageBox msgBox;
        msgBox.setText(QStringLiteral("New Folder (under %1)").arg(pFolder->text(0)));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

QAction* LibraryView::CreateSortMenuItem(const QString& text, int data_, bool isChecked)
{
    QAction* pAction = new QAction(text, this);
    pAction->setData(data_);
    pAction->setCheckable(true);
    pAction->setChecked(isChecked);
    return pAction;
}

#define CONTEXT_MENU_DELETE        -1
#define CONTEXT_MENU_RENAME        -2

void LibraryView::on_iconView_customContextMenuRequested(const QPoint &pos)
{
    if (ui->treeWidget->selectedItems().size())
    {
        QModelIndexList selectedIndexes = ui->iconView->selectionModel()->selectedIndexes();
        if (selectedIndexes.size() > 0)
        {
            QMenu menu(this);

            menu.addAction(tr("Delete Item"), this, SLOT(OnDeleteDesignIconView(bool)));
            menu.addAction(tr("Rename Item"), this, SLOT(OnRenameDesignIconView(bool)));

            menu.exec(ui->iconView->mapToGlobal(pos));
        }
    }
}

void LibraryView::OnDeleteDesignIconView(bool /*checked*/)
{
    QModelIndexList selectedIndexes = ui->iconView->selectionModel()->selectedIndexes();
    if (selectedIndexes.size() > 0)
    {
        QModelIndex index = selectedIndexes[0];

        QMessageBox msgBox;
        msgBox.setText(QStringLiteral("Delete Design (%1)").arg(index.data(Qt::EditRole).toString()));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

void LibraryView::OnRenameDesignIconView(bool /*checked*/)
{
    QModelIndexList selectedIndexes = ui->iconView->selectionModel()->selectedIndexes();
    if (selectedIndexes.size() > 0)
    {
        QModelIndex index = selectedIndexes[0];
        //const QAbstractItemModel* pItem = index.model();
        
        QMessageBox msgBox;
        msgBox.setText(QStringLiteral("Rename Design (%1)").arg(index.data(Qt::EditRole).toString()));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

void LibraryView::on_listView_customContextMenuRequested(const QPoint &pos)
{
    if (ui->treeWidget->selectedItems().size())
    {
        QMenu menu(this);

        QModelIndexList selectedIndexes = ui->listView->selectionModel()->selectedIndexes();
        if (selectedIndexes.size() > 0)
        {
            menu.addAction(tr("Delete Item"), this, SLOT(OnDeleteDesignListView(bool)));
            menu.addAction(tr("Rename Item"), this, SLOT(OnRenameDesignListView(bool)));
            menu.addSeparator();
        }

        int sortColumn = ui->listView->header()->sortIndicatorSection();

        QMenu* pSortByMenu = menu.addMenu(tr("Sort by"));
            pSortByMenu->addAction(CreateSortMenuItem(tr("Sort by Name"), 1, sortColumn == 0));
            pSortByMenu->addAction(CreateSortMenuItem(tr("Sort by Type"), 2, sortColumn == 1));
            pSortByMenu->addAction(CreateSortMenuItem(tr("Sort by Artist"), 3, sortColumn == 2));
            pSortByMenu->addAction(CreateSortMenuItem(tr("Sort by Modified Date"), 4, sortColumn == 2)); //-V112
            //pSortByMenu->addAction(CreateSortMenuItem(pSortBy, tr("Sort by Size"), 5, sortColumn == 2));

        QAction* pAction = menu.exec(ui->listView->mapToGlobal(pos));
        if (pAction)
        {
            bool success = false; 
            int column = pAction->data().toInt(&success);
            if (success && column != sortColumn && column > 1 && column <= 4) //-V112
            {
                ui->treeWidget->sortByColumn(column - 1);
            }
        }
    }
}

void LibraryView::OnDeleteDesignListView(bool /*checked*/)
{
    QModelIndexList selectedIndexes = ui->listView->selectionModel()->selectedIndexes();
    if (selectedIndexes.size() > 0)
    {
        QModelIndex index = selectedIndexes[0];
        //const QAbstractItemModel* pItem = index.model();
        
        QMessageBox msgBox;
        msgBox.setText(QStringLiteral("Delete Design (%1)").arg(index.data(Qt::EditRole).toString()));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

void LibraryView::OnRenameDesignListView(bool /*checked*/)
{
    QModelIndexList selectedIndexes = ui->listView->selectionModel()->selectedIndexes();
    if (selectedIndexes.size() > 0)
    {
        QModelIndex index = selectedIndexes[0];
        //const QAbstractItemModel* pItem = index.model();
        
        QMessageBox msgBox;
        msgBox.setText(QStringLiteral("Rename Design (%1)").arg(index.data(Qt::EditRole).toString()));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
}

void LibraryView::OnColumnResized(int column, int /*oldSize*/, int newSize)
{
    if (column > 1 && column < 4) //-V112
    {
        pSettings->setValue(settingsPath_columns % QString::number(column), newSize);
    }
}

void LibraryView::OnColumnSortChanged(int column, Qt::SortOrder order)
{
    column = column + 1;
    if (order == Qt::DescendingOrder)
    {
        column = -column;
    }
    pSettings->setValue(settingsPath_columnSort, column);
}

///////////////////////////////////////////////////////////////////////////

LibrarySqlModel::LibrarySqlModel(QObject* pParent)
    : QSqlQueryModel(pParent)
    , m_isIconView(true)
{
    SetIconView(m_isIconView);
}

LibrarySqlModel::~LibrarySqlModel()
{
}

QVariant LibrarySqlModel::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);
/*
    switch (role)
    {
        case Qt::DecorationRole:
            switch (index.column())
            {
                case 0:
                {
                    QByteArray data = record(index.row()).value(4).toByteArray();
                    if (!data.isEmpty())
                    {
                        QPixmap image;
                        if (image.loadFromData(data))
                            return image.scaled(m_viewIconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    }

                    QPixmap image(m_viewIconSize);
                    image.fill(Qt::transparent);

                    if (GetDesignTypeByValue(record(index.row()).value(1).toUInt()) == DesignType::Font)
                    {
                        QFont font;

                        QStringList fontNames = record(index.row()).value(0).toString().split(QChar('|'), QString::SkipEmptyParts);
                        switch (fontNames.size())
                        {
                            case 1:
                                font.setFamily(fontNames[0]);
                                break;

                            case 2:
                                font.setFamily(fontNames[1]);
                                break;

                            case 3:
                                font.setFamily(fontNames[1]);
                                font.setStyleName(fontNames[2]);
                                break;
                        }

                        bool isMatch = font.exactMatch();
                        if (!isMatch && fontNames.size() == 3)
                            font.setStyleName(QStringLiteral(""));

                        if (isMatch || font.exactMatch())
                        {
                            font.setPointSize(m_fontSize);

                            QPainter painter;

                            painter.begin(&image);
                            if (painter.isActive())
                            {
                                painter.setFont(font);
                                painter.setPen(Qt::black);
                                painter.drawText(0, 0, image.width(), image.height(), Qt::AlignHCenter | Qt::AlignVCenter, QStringLiteral("AaBb"));
                            }
                            painter.end();
                        }
                    }

                    return image;

                    //QFontDatabase fontDatabase; 
                    //fontDatabase.addApplicationFont(":/fonts/yourfont");
                }
            }
            break;

        //case Qt::TextAlignmentRole:
        //    switch (index.column())
        //    {
        //        case 0:
        //            return Qt::AlignLeft;
        //    }
        //    break;

        case Qt::DisplayRole:
        case Qt::ToolTipRole:
            if (value.isValid())
            {
                switch (index.column())
                {
                    case 0: // name
                        //if (GetDesignTypeByValue(record(index.row()).value(1).toUInt()) == DesignType::Font)
                        //{
                        //    QString fontName = value.toString();
                        //    int index = fontName.indexOf(QChar('|'));
                        //    if (index > 0)
                        //        return fontName.left(index);
                        //}
                        break;

                    case 1: // type
//                        return GetDesignTypeNameByValue(value.toInt());
                        break;

                    case 2: // artist
                        break;

                    case 3: // date
                    {
                        QDateTime modifiedDate;
                        qint64 msecs = value.toLongLong();
                        modifiedDate.setMSecsSinceEpoch(msecs);
                        return modifiedDate;
                    }
                }
            }
            break;
    }

//   if (role == Qt::TextColorRole && index.column() == 1)
//       return QVariant::fromValue(QColor(Qt::blue));
    */
    return value;
}

///////////////////////////////////////////////////////////////////////////

FolderItemDelegate::FolderItemDelegate(QObject* pParent)
    : QStyledItemDelegate(pParent)
{
}

FolderItemDelegate::~FolderItemDelegate()
{
}

void FolderItemDelegate::paint(QPainter* pPainter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::paint(pPainter, option, index);
}

QSize FolderItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QSize sz = QStyledItemDelegate::sizeHint(option, index);
    return sz;
}

QWidget* FolderItemDelegate::createEditor(QWidget* pParent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QStyledItemDelegate::createEditor(pParent, option, index);
}

void FolderItemDelegate::setEditorData(QWidget* pEditor, const QModelIndex& index) const
{
    QVariant v = index.data(Qt::UserRole + 1);
    QByteArray n = pEditor->metaObject()->userProperty().name();

    if (!n.isEmpty())
    {
        if (!v.isValid())
        {
            v = QVariant(pEditor->property(n).userType(), nullptr);
        }
        pEditor->setProperty(n, v);
    }
}

void FolderItemDelegate::setModelData(QWidget* pEditor, QAbstractItemModel* pModel, const QModelIndex &index) const
{
    QByteArray n = pEditor->metaObject()->userProperty().name();
    if (!n.isEmpty())
    {
        QString newName = pEditor->property(n).toString();
        if (!newName.isEmpty())
        {
            pModel->setData(index, newName, Qt::UserRole + 1);

            int subTotalDesigns = index.data(Qt::UserRole + 2).toInt();
            static QString suffixStr = QStringLiteral(" (%L1)");
            QString text = newName % suffixStr.arg(subTotalDesigns);

            pModel->setData(index, text, Qt::EditRole);
        }
    }
}
