
#include "modeltablestock.h"

ModelTableStock::ModelTableStock(QObject *parent)
    : QAbstractTableModel(parent)
{
    tableHeader<<"代码"<<"名称"<<"涨跌幅"<<"最新价"<<"买一"<<"卖一"<<"换手率"<<"成交额"<<"总市值"<<"市盈率"<<"流通市值"<<"振幅"<<"量比"<<"今年"<<"60日"
                <<"成交量"<< "最高"<< "最低"<< "今开"<<"昨收"<<"涨速"<<"市净率"<<"总营收"<< "总营收同比"<< "净利润"<< "净利润同比"<<"毛利率"<<"净利率"
                <<"净资产"<<"ROE"<<"行业"<<"地域"<<"概念板块"<<"上市时间";
}

void ModelTableStock::setModelData(const QList<StockInfo> &data,bool forced,bool marking)
{
    mark=marking;
    if (m_modelData.size()!=data.size() or forced)
    {
        beginResetModel();
        m_modelData = data;
        endResetModel();
    }
    else
    {
        m_modelData = data;
        emit dataChanged(index(0,0),index(rowCount()-1,columnCount()-1));
    }
}

int ModelTableStock::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_modelData.size();
}

int ModelTableStock::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return tableHeader.size();
}

QVariant ModelTableStock::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) // 返回一个默认数据的表头
        return QAbstractTableModel::headerData(section, orientation, role);

    // 仅仅只是返回水平方向的表头
    if (orientation == Qt::Orientation::Horizontal) {
        return tableHeader[section];
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

QVariant ModelTableStock::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        //DisplayRole返回显示的文本值
        const int row = index.row();
        switch(index.column())
        {
        case 0: return m_modelData.at(row).code;
        case 1: return m_modelData.at(row).name;
        case 2: return GlobalVar::format_conversion(m_modelData.at(row).pctChg)+"%";
        case 3:
        {
            int d=2;
            if (GlobalVar::WhichInterface==6 or GlobalVar::WhichInterface==2)
                d=3;
            return QString::number(m_modelData.at(row).close,'f',d);
        }
        case 4:
            if (m_modelData.at(row).buy1==0)
                return "-    ";
            else
                return QString::number(m_modelData.at(row).buy1,'f',2);
        case 5:
            if (m_modelData.at(row).sell1==0)
                return "-    ";
            else
                return QString::number(m_modelData.at(row).sell1,'f',2);
        case 6: return m_modelData.at(row).turn;
        case 7: return GlobalVar::format_conversion(m_modelData.at(row).amount);
        case 8: return GlobalVar::format_conversion(m_modelData.at(row).totalValue);
        case 9: return QString::number(m_modelData.at(row).pe,'f',2);
        case 10: return GlobalVar::format_conversion(m_modelData.at(row).circulatedValue);
        case 11: return QString::number(m_modelData.at(row).amplitude,'f',2)+"%";
        case 12: return QString::number(m_modelData.at(row).qrr,'f',2);
        case 13: return QString::number(m_modelData.at(row).pctYear,'f',2)+"%";
        case 14: return QString::number(m_modelData.at(row).pctSixty,'f',2)+"%";
        case 15: return GlobalVar::format_conversion(m_modelData.at(row).volume);
        case 16: return QString::number(m_modelData.at(row).high,'f',2);
        case 17: return QString::number(m_modelData.at(row).low,'f',2);
        case 18: return QString::number(m_modelData.at(row).open,'f',2);
        case 19: return QString::number(m_modelData.at(row).preClose,'f',2);
        case 20: return QString::number(m_modelData.at(row).velocity,'f',2);
        case 21: return QString::number(m_modelData.at(row).pbr,'f',2);
        case 22: return GlobalVar::format_conversion(m_modelData.at(row).totalRevenue);
        case 23: return QString::number(m_modelData.at(row).totalRevenueOn,'f',2)+"%";
        case 24: return GlobalVar::format_conversion(m_modelData.at(row).netProfit);
        case 25: return QString::number(m_modelData.at(row).netProfitOn,'f',2)+"%";
        case 26: return QString::number(m_modelData.at(row).grossProfitMargin,'f',2)+"%";
        case 27: return QString::number(m_modelData.at(row).netProfitMargin,'f',2)+"%";
        case 28: return GlobalVar::format_conversion(m_modelData.at(row).netAssets);
        case 29: return QString::number(m_modelData.at(row).roe,'f',2)+"%";
        case 30: return m_modelData.at(row).industry;
        case 31: return m_modelData.at(row).region;
        case 32: return m_modelData.at(row).concept;
        case 33: return m_modelData.at(row).listingDate;
        }
    }
    else if (role == Qt::ForegroundRole)
    {
        const int row = index.row();
        switch(index.column())
        {
        case 0:
            return QColor(128,128,0);
        case 1:
            if (mark)
            {
                for (int i=0;i<GlobalVar::mMyStockCode.count();++i)
                {
                    if (m_modelData.at(row).code==GlobalVar::mMyStockCode[i])
                        return QColor(255,140,0);
                }
            }
            return QColor(72,61,139);
        case 2:
            if (m_modelData.at(row).pctChg>0)
                return QColor(Qt::red);
            else if (m_modelData.at(row).pctChg<0)
                return QColor(0, 191, 0);
            break;
        case 3:
            if (m_modelData.at(row).close>m_modelData.at(row).open)
                return QColor(255, 0, 255);
            else if (m_modelData.at(row).close<m_modelData.at(row).open)
                return QColor(0, 191, 0);
            break;
        case 4:
            if (m_modelData.at(row).buy1>m_modelData.at(row).open)
                return QColor(255, 130, 255);
            else if (m_modelData.at(row).buy1<m_modelData.at(row).open)
                return QColor(0, 191, 0);
            break;
        case 5:
            if (m_modelData.at(row).sell1>m_modelData.at(row).open)
                return QColor(255, 130, 255);
            else if (m_modelData.at(row).sell1<m_modelData.at(row).open)
                return QColor(0, 191, 0);
            break;
        case 6:
            if (m_modelData.at(row).turn>=15)
                return QColor(204, 204, 0);
            break;
        case 7:
            if (m_modelData.at(row).amount>= 1000000000)
                return QColor(153, 0, 153);
            else if (m_modelData.at(row).amount >= 300000000)
                return QColor(0, 191, 255);
            break;
        case 8:
            if (m_modelData.at(row).totalValue/100>100000000)
                return QColor(32,178,170);
            break;
        case 9:
            if (m_modelData.at(row).pe<0)
                return QColor(0, 191, 0);
            break;
        case 10:
            if (m_modelData.at(row).circulatedValue/100>100000000)
                return QColor(32,178,170);
            break;
        case 11:
            if (m_modelData.at(row).amplitude>=10)
                return QColor(205,133,63);
            break;
        case 12:
            if (m_modelData.at(row).qrr>=2)
                return QColor(30,144,255);
            break;
        case 13:
            if (m_modelData.at(row).pctYear >= 100)
                return QColor(153, 0, 153);
            else if (m_modelData.at(row).pctYear < 100 and m_modelData.at(row).pctYear >0)
                return QColor(255, 155, 153);
            else if (m_modelData.at(row).pctYear < 0)
                return QColor(0, 191, 0);
            break;
        case 14:
            if (m_modelData.at(row).pctSixty >= 100)
                return QColor(153, 0, 153);
            else if (m_modelData.at(row).pctSixty < 100 and m_modelData.at(row).pctSixty >0)
                return QColor(255, 155, 153);
            else if (m_modelData.at(row).pctSixty < 0)
                return QColor(0, 191, 0);
            break;
        case 16:
            if (m_modelData.at(row).high>m_modelData.at(row).preClose)
                return QColor(Qt::red);
            else if (m_modelData.at(row).high<m_modelData.at(row).preClose)
                return QColor(0, 191, 0);
            break;
        case 17:
            if (m_modelData.at(row).low>m_modelData.at(row).preClose)
                return QColor(Qt::red);
            else if (m_modelData.at(row).low<m_modelData.at(row).preClose)
                return QColor(0, 191, 0);
            break;
        case 18:
            if (m_modelData.at(row).close>m_modelData.at(row).open)
                    return QColor(255, 0, 255);
            else if (m_modelData.at(row).close<m_modelData.at(row).open)
                    return QColor(0, 191, 0);
            break;
        case 20:
            if (m_modelData.at(row).velocity>= 2)
                return QColor(153, 0, 153);
            else if (m_modelData.at(row).velocity >0)
                return QColor(Qt::red);
            else if (m_modelData.at(row).velocity < 0)
                return QColor(0, 191, 0);
            break;
        case 21:
            if (m_modelData.at(row).pbr<0)
                return QColor(0, 191, 0);
            break;
        case 23:
            if (m_modelData.at(row).totalRevenueOn<0)
                return QColor(0, 191, 0);
            break;
        case 24:
            if (m_modelData.at(row).netProfit<0)
                return QColor(0, 191, 0);
            break;
        case 25:
            if (m_modelData.at(row).netProfitOn<0)
                return QColor(0, 191, 0);
            break;
        case 26:
            if (m_modelData.at(row).grossProfitMargin<0)
                return QColor(0, 191, 0);
            break;
        case 27:
            if (m_modelData.at(row).netProfitMargin<0)
                return QColor(0, 191, 0);
            break;
        case 28:
            if (m_modelData.at(row).netAssets<0)
                return QColor(0, 191, 0);
            break;
        case 29:
            if (m_modelData.at(row).roe<0)
                return QColor(0, 191, 0);
            break;
        }
    }
    else if (role == Qt::FontRole)
    {
        QFont boldFont = QFont();
        if (index.column() == 1)
            boldFont.setFamily("宋体");
        if (index.column() == 2 or index.column() == 1 or index.column() == 7)
        {
            boldFont.setBold(true);
            boldFont.setPixelSize(16);
            return boldFont;
        }
    }
    else if (role == Qt::TextAlignmentRole)
    {
        switch(index.column())
        {
        case 2:return Qt::AlignRight;
        case 3:return Qt::AlignRight;
        case 4:return Qt::AlignRight;
        case 5:return Qt::AlignRight;
        case 6:return Qt::AlignRight;
        case 7:return Qt::AlignRight;
        case 8:return Qt::AlignRight;
        case 9:return Qt::AlignRight;
        case 10:return Qt::AlignRight;
        case 11:return Qt::AlignRight;
        case 12:return Qt::AlignRight;
        case 13:return Qt::AlignRight;
        case 14:return Qt::AlignRight;
        case 15:return Qt::AlignRight;
        case 16:return Qt::AlignRight;
        case 17:return Qt::AlignRight;
        case 18:return Qt::AlignRight;
        case 19:return Qt::AlignRight;
        case 20:return Qt::AlignCenter;
        case 21:return Qt::AlignRight;
        case 22:return Qt::AlignRight;
        case 23:return Qt::AlignRight;
        case 24:return Qt::AlignRight;
        case 25:return Qt::AlignRight;
        case 26:return Qt::AlignRight;
        case 27:return Qt::AlignRight;
        case 28:return Qt::AlignRight;
        case 29:return Qt::AlignRight;
        }
    }
    return QVariant();
}

