#ifndef PRJWIZARD_H
#define PRJWIZARD_H

#include "dialogresult.h"
#include <QDialog>
#include <QListWidgetItem>

namespace Ui
{
    class PrjWizard;
}

class PrjSetupDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit PrjSetupDialog(QWidget *parent = 0);
        ~PrjSetupDialog();

        DialogResult showDialog();

        inline Ui::PrjWizard* getUI() const;

    private:
        Ui::PrjWizard *ui;
        DialogResult lastresult;
};


Ui::PrjWizard* PrjSetupDialog::getUI() const
{
    return ui;
}

#endif // PRJWIZARD_H
