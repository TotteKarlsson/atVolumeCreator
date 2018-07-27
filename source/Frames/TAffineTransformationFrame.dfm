inherited AffineTransformationFrame: TAffineTransformationFrame
  Width = 396
  Height = 480
  ExplicitWidth = 396
  ExplicitHeight = 480
  inherited GroupBox1: TGroupBox
    Width = 396
    ExplicitWidth = 396
    inherited RenderClientScriptsFolderE: TSTDStringLabeledEdit
      EditLabel.ExplicitWidth = 124
    end
  end
  inherited Panel2: TPanel
    Width = 396
    Height = 311
    ExplicitWidth = 396
    ExplicitHeight = 311
    inherited StacksGB: TGroupBox
      Height = 309
      ExplicitHeight = 309
      inherited StacksForProjectCB: TCheckListBox
        Height = 286
        ExplicitHeight = 286
      end
    end
    inherited GroupBox2: TGroupBox
      Width = 209
      Height = 309
      ExplicitWidth = 209
      ExplicitHeight = 309
      DesignSize = (
        209
        309)
      object AppendToCurrentStackCB: TPropertyCheckBox [0]
        Left = 10
        Top = 24
        Width = 129
        Height = 33
        Caption = 'Append Transform to Current Stack'
        Checked = True
        State = cbChecked
        TabOrder = 0
        WordWrap = True
      end
      object RotationE: TFloatLabeledEdit [1]
        Left = 10
        Top = 75
        Width = 97
        Height = 21
        EditLabel.Width = 70
        EditLabel.Height = 13
        EditLabel.Caption = 'Rotation (deg)'
        TabOrder = 1
        Text = '0'
      end
      object TranslateXE: TIntegerLabeledEdit [2]
        Left = 10
        Top = 123
        Width = 97
        Height = 21
        EditLabel.Width = 92
        EditLabel.Height = 13
        EditLabel.Caption = 'Translate X (pixels)'
        TabOrder = 2
        Text = '0'
      end
      object TranslateYE: TIntegerLabeledEdit [3]
        Left = 10
        Top = 171
        Width = 97
        Height = 21
        EditLabel.Width = 92
        EditLabel.Height = 13
        EditLabel.Caption = 'Translate Y (pixels)'
        TabOrder = 3
        Text = '0'
      end
      inherited ExecuteBtn: TButton
        Left = 10
        Top = 210
        TabOrder = 4
        OnClick = ExecuteBtnClick
        ExplicitLeft = 10
        ExplicitTop = 210
      end
    end
  end
end
