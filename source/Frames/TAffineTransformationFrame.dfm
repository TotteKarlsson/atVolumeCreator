inherited AffineTransformationFrame: TAffineTransformationFrame
  inherited GroupBox1: TGroupBox
    inherited RenderClientScriptsFolderE: TSTDStringLabeledEdit
      EditLabel.ExplicitWidth = 124
    end
    inherited ExecuteBtn: TButton
      OnClick = ExecuteBtnClick
    end
  end
  inherited Panel2: TPanel
    inherited GroupBox2: TGroupBox
      ExplicitLeft = 186
      ExplicitTop = 1
      ExplicitWidth = 512
      ExplicitHeight = 405
      object AppendToCurrentStackCB: TPropertyCheckBox
        Left = 24
        Top = 32
        Width = 209
        Height = 17
        Caption = 'Append Transform to Current Stack'
        Checked = True
        State = cbChecked
        TabOrder = 0
      end
      object TransformID: TSTDStringLabeledEdit
        Left = 24
        Top = 80
        Width = 121
        Height = 21
        EditLabel.Width = 60
        EditLabel.Height = 13
        EditLabel.Caption = 'TransformID'
        TabOrder = 1
        Text = 'GlobalAffine'
        Value = 'GlobalAffine'
      end
      object PoolSizeE: TIntegerLabeledEdit
        Left = 24
        Top = 136
        Width = 121
        Height = 21
        EditLabel.Width = 45
        EditLabel.Height = 13
        EditLabel.Caption = 'PoolSizeE'
        TabOrder = 2
        Text = '0'
      end
      object RotationE: TFloatLabeledEdit
        Left = 192
        Top = 80
        Width = 97
        Height = 21
        EditLabel.Width = 70
        EditLabel.Height = 13
        EditLabel.Caption = 'Rotation (deg)'
        TabOrder = 3
        Text = '0'
      end
      object TranslateXE: TIntegerLabeledEdit
        Left = 192
        Top = 136
        Width = 97
        Height = 21
        EditLabel.Width = 92
        EditLabel.Height = 13
        EditLabel.Caption = 'Translate X (pixels)'
        TabOrder = 4
        Text = '0'
      end
      object TranslateYE: TIntegerLabeledEdit
        Left = 320
        Top = 136
        Width = 97
        Height = 21
        EditLabel.Width = 92
        EditLabel.Height = 13
        EditLabel.Caption = 'Translate Y (pixels)'
        TabOrder = 5
        Text = '0'
      end
    end
  end
end
