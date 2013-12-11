#ifndef SAVEMODIFIEDLAYERSDIALOG_H
#define SAVEMODIFIEDLAYERSDIALOG_H

#include <QDialog>

namespace Ui {
class SaveModifiedLayersDialog;
}

class SaveModifiedLayersModel;
class EventBucket;
class GlobalUIModel;
class QAbstractButton;
class ImageWrapperBase;

class SaveModifiedLayersDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit SaveModifiedLayersDialog(QWidget *parent = 0);
  ~SaveModifiedLayersDialog();

  void SetModel(SaveModifiedLayersModel *model);

  /**
   * Flags affecting the dialog's behavior
   */
  enum PromptOption
  {
    NoOption = 0x00,
    DiscardDisabled = 0x01,
    ProjectsDisabled = 0x02
  };

  Q_DECLARE_FLAGS(PromptOptions, PromptOption)

  void SetOptions(PromptOptions opts);

  /**
   * This static method prompts user to save unsaved changes to all the layers,
   * and return true if the user indicated that he/she wishes to proceed with
   * the subsequent operation. It is also possible to request the dialog for a
   * single image layer (e.g. segmentation)
   *
   * With only the first parameter passed (the model), the function creates a
   * dialog appropriate when the main layer is being closed, i.e., when the
   * user presses quit. It shows all unsaved layers as well as the workspace
   * if it exists and is unsaved.
   */
  static bool PromptForUnsavedChanges(GlobalUIModel *model,
                                      ImageWrapperBase *singleLayer = NULL,
                                      PromptOptions opts = NoOption,
                                      QWidget *parent = NULL);

  static bool PromptForUnsavedSegmentationChanges(GlobalUIModel *model);


private slots:

  void onModelUpdate(const EventBucket &bucket);
  
  void on_buttonBox_clicked(QAbstractButton *button);

private:
  Ui::SaveModifiedLayersDialog *ui;

  SaveModifiedLayersModel *m_Model;

  void UpdateUnsavedTable();
};

Q_DECLARE_OPERATORS_FOR_FLAGS(SaveModifiedLayersDialog::PromptOptions)

#endif // SAVEMODIFIEDLAYERSDIALOG_H
