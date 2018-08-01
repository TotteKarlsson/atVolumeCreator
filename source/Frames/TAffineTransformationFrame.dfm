inherited AffineTransformationFrame: TAffineTransformationFrame
  Width = 392
  Height = 533
  ExplicitWidth = 392
  ExplicitHeight = 533
  inherited GroupBox1: TGroupBox
    Width = 392
    ExplicitWidth = 396
    inherited RenderClientScriptsFolderE: TSTDStringLabeledEdit
      EditLabel.ExplicitWidth = 124
    end
  end
  inherited Panel2: TPanel
    Width = 392
    Height = 364
    ExplicitWidth = 396
    ExplicitHeight = 311
    inherited StacksGB: TGroupBox
      Height = 362
      ExplicitHeight = 309
      inherited StacksForProjectCB: TCheckListBox
        Height = 339
        Flat = False
        PopupMenu = StacksPopup
        Sorted = True
        ExplicitHeight = 339
      end
    end
    inherited GroupBox2: TGroupBox
      Width = 205
      Height = 362
      ExplicitLeft = 187
      ExplicitTop = 6
      ExplicitWidth = 209
      ExplicitHeight = 362
      DesignSize = (
        205
        362)
      object TranslateYE: TIntegerLabeledEdit [0]
        Left = 10
        Top = 188
        Width = 97
        Height = 21
        EditLabel.Width = 92
        EditLabel.Height = 13
        EditLabel.Caption = 'Translate Y (pixels)'
        TabOrder = 3
        Text = '0'
      end
      object TranslateXE: TIntegerLabeledEdit [1]
        Left = 10
        Top = 140
        Width = 97
        Height = 21
        EditLabel.Width = 92
        EditLabel.Height = 13
        EditLabel.Caption = 'Translate X (pixels)'
        TabOrder = 2
        Text = '0'
      end
      object RotationE: TFloatLabeledEdit [2]
        Left = 10
        Top = 92
        Width = 97
        Height = 21
        EditLabel.Width = 70
        EditLabel.Height = 13
        EditLabel.Caption = 'Rotation (deg)'
        TabOrder = 1
        Text = '0'
      end
      object AppendToCurrentStackCB: TPropertyCheckBox [3]
        Left = 10
        Top = 24
        Width = 95
        Height = 41
        Caption = 'Append Transform to Current Stack'
        TabOrder = 0
        WordWrap = True
      end
      inherited ExecuteBtn: TButton
        Left = 9
        Top = 227
        Width = 95
        TabOrder = 4
        OnClick = ExecuteBtnClick
        ExplicitLeft = 10
        ExplicitTop = 227
        ExplicitWidth = 95
      end
    end
  end
  object StacksPopup: TPopupMenu
    OnPopup = StacksPopupPopup
    Left = 80
    Top = 272
    object DeleteStack1: TMenuItem
      Action = DeleteStackA
    end
  end
  object StacksActions: TActionList
    Left = 64
    Top = 360
    object DeleteStackA: TAction
      Caption = 'Delete Stack'
      OnExecute = DeleteStackAExecute
    end
  end
end
