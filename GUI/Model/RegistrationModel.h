#ifndef REGISTRATIONMODEL_H
#define REGISTRATIONMODEL_H

#include "AbstractModel.h"
#include "PropertyModel.h"

class GlobalUIModel;
class IRISApplication;
class ImageWrapperBase;

namespace itk
{
  template<typename T, unsigned int D1, unsigned int D2> class Matrix;
  template<typename T, unsigned int D1> class Vector;
}

class RegistrationModel : public AbstractModel
{
public:
  irisITKObjectMacro(RegistrationModel, AbstractModel)

  /**
    States in which the model can be, which allow the activation and
    deactivation of various widgets in the interface
    */
  enum UIState {
    UIF_MOVING_SELECTION_AVAILABLE,
    UIF_MOVING_SELECTED
  };

  /**
    Check the state flags above
    */
  bool CheckState(UIState state);

  void SetParentModel(GlobalUIModel *model);
  irisGetMacro(Parent, GlobalUIModel *)

  virtual void OnUpdate();

  typedef SimpleItemSetDomain<unsigned long, std::string> LayerSelectionDomain;
  typedef AbstractPropertyModel<unsigned long, LayerSelectionDomain> AbstractLayerSelectionModel;

  /** Model for selecting the layer for registration */
  irisGetMacro(MovingLayerModel, AbstractLayerSelectionModel *)

  /** Euler angles */
  irisGetMacro(EulerAnglesModel, AbstractRangedDoubleVec3Property *)

  /** Euler angles */
  irisGetMacro(TranslationModel, AbstractRangedDoubleVec3Property *)

  /** Interactive registration tool button */
  irisGetMacro(InteractiveToolModel, AbstractSimpleBooleanProperty *)

  /** Set the center of rotation to current cross-hairs position */
  void SetCenterOfRotationToCursor();

  /** Reset the rotation to identity */
  void ResetTransformToIdentity();

  /** Apply a rotation around a fixed angle */
  void ApplyRotation(const Vector3d &axis, double theta);

  /** Get a pointer to the selected moving wrapper, or NULL if none selected */
  ImageWrapperBase *GetMovingLayerWrapper();

  /** Get the center of rotation, in voxel units of the main image */
  irisGetMacro(RotationCenter, Vector3ui)

protected:
  RegistrationModel();
  ~RegistrationModel();

  typedef itk::Matrix<double, 3, 3> ITKMatrixType;
  typedef itk::Vector<double, 3> ITKVectorType;

  GlobalUIModel *m_Parent;
  IRISApplication *m_Driver;

  void ResetOnMainImageChange();

  void UpdateManualParametersFromWrapper(bool force_update = false);
  void UpdateWrapperFromManualParameters();

  void SetRotationCenter(const Vector3ui &pos);


  SmartPtr<AbstractLayerSelectionModel> m_MovingLayerModel;
  bool GetMovingLayerValueAndRange(unsigned long &value, LayerSelectionDomain *range);
  void SetMovingLayerValue(unsigned long value);

  SmartPtr<AbstractSimpleBooleanProperty> m_InteractiveToolModel;
  bool GetInteractiveToolValue(bool &value);
  void SetInteractiveToolValue(bool value);

  SmartPtr<AbstractRangedDoubleVec3Property> m_EulerAnglesModel;
  bool GetEulerAnglesValueAndRange(Vector3d &value, NumericValueRange<Vector3d> *range);
  void SetEulerAnglesValue(Vector3d value);

  SmartPtr<AbstractRangedDoubleVec3Property> m_TranslationModel;
  bool GetTranslationValueAndRange(Vector3d &value, NumericValueRange<Vector3d> *range);
  void SetTranslationValue(Vector3d value);

  // Value corresponding to no layer selected
  static const unsigned long NOID;

  // The active layer for the segmentation
  unsigned long m_MovingLayerId;

  // The components of the transform that are presented to the user by this widget
  struct TransformManualParameters
  {
    // Euler angles
    Vector3d EulerAngles;

    // Translation
    Vector3d Translation;

    // Range of translation
    NumericValueRange<Vector3d> TranslationRange;

    // The unique layer ID for which this data was computed
    unsigned long LayerID;

    // Time stamp of the last update
    itk::TimeStamp UpdateTime;

    TransformManualParameters() : LayerID(NOID) {}
  };

  // The current cached manual parameters
  TransformManualParameters m_ManualParam;

  // Current center of rotation - should be initialized to the center when new image is loaded
  Vector3ui m_RotationCenter;
};

#endif // REGISTRATIONMODEL_H
