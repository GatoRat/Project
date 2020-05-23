// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#pragma once

class Model : public QAbstractItemModel
{
    Q_OBJECT

private:
    struct Node
    {
        Node(Node *parent = 0) : parent(parent), children(0) {}
        ~Node() { delete children; }
        Node *parent;
        QVector<Node> *children;
    };

    Node *node(int row, Node *parent) const;
    Node *parent(Node *child) const;
    int row(Node *node) const;

    QIcon services;
    int rc;
    int cc;
    QVector<Node> *tree;

public:
    Model(int rows, int columns, QObject *parent = 0);
    ~Model();

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool hasChildren(const QModelIndex &parent) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
};
