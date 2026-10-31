#include "NameDialog.h"
#include "ui_NameDialog.h"

NameDialog::NameDialog(const QString& title,
                       const QString& initialName,
                       QWidget* parent)
    : QDialog(parent), ui(new Ui::NameDialog) {
    ui->setupUi(this);
    setWindowTitle(title);
    ui->hintLabel->setText(title);
    ui->nameEdit->setText(initialName);
    ui->nameEdit->selectAll();
}

NameDialog::~NameDialog() {
    delete ui;
}

QString NameDialog::name() const {
    return ui->nameEdit->text().trimmed();
}
