#pragma once
#include <QDialog>

namespace Ui { class NameDialog; }

class NameDialog : public QDialog {
    Q_OBJECT
public:
    explicit NameDialog(const QString& title,
                        const QString& initialName,
                        QWidget* parent = nullptr);
    ~NameDialog() override;

    QString name() const;

private:
    Ui::NameDialog* ui;
};
