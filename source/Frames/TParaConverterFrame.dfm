object ParaConverterFrame: TParaConverterFrame
  Left = 0
  Top = 0
  Width = 556
  Height = 500
  Constraints.MinHeight = 500
  TabOrder = 0
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 556
    Height = 500
    Align = alClient
    Caption = 'Settings'
    TabOrder = 0
    ExplicitTop = 70
    ExplicitHeight = 430
    object GroupBox2: TGroupBox
      Left = 2
      Top = 101
      Width = 552
      Height = 217
      Align = alTop
      Caption = 'Box generation'
      TabOrder = 0
      object BoxHeightE: TIntegerLabeledEdit
        Left = 16
        Top = 83
        Width = 121
        Height = 21
        EditLabel.Width = 31
        EditLabel.Height = 13
        EditLabel.Caption = 'Height'
        TabOrder = 0
        Text = '512'
        Value = 512
      end
      object BoxWidthE: TIntegerLabeledEdit
        Left = 16
        Top = 36
        Width = 121
        Height = 21
        EditLabel.Width = 28
        EditLabel.Height = 13
        EditLabel.Caption = 'Width'
        TabOrder = 1
        Text = '512'
        Value = 512
      end
      object BoxDepthE: TIntegerLabeledEdit
        Left = 16
        Top = 130
        Width = 121
        Height = 21
        EditLabel.Width = 29
        EditLabel.Height = 13
        EditLabel.Caption = 'Depth'
        TabOrder = 2
        Text = '512'
        Value = 512
      end
      object OutputFolderE: TSTDStringLabeledEdit
        Left = 16
        Top = 180
        Width = 433
        Height = 21
        EditLabel.Width = 67
        EditLabel.Height = 13
        EditLabel.Caption = 'Output Folder'
        TabOrder = 3
      end
    end
    object GroupBox3: TGroupBox
      Left = 2
      Top = 15
      Width = 552
      Height = 86
      Align = alTop
      Caption = 'Input'
      TabOrder = 1
      object InputFolderE: TSTDStringLabeledEdit
        Left = 16
        Top = 36
        Width = 433
        Height = 21
        EditLabel.Width = 66
        EditLabel.Height = 13
        EditLabel.Caption = 'Source Folder'
        TabOrder = 0
      end
    end
    object RunGB: TGroupBox
      Left = 2
      Top = 318
      Width = 552
      Height = 180
      Align = alClient
      Caption = 'Run'
      TabOrder = 2
      ExplicitHeight = 110
      object NumberOfProcessesE: TIntegerLabeledEdit
        Left = 16
        Top = 51
        Width = 121
        Height = 21
        EditLabel.Width = 134
        EditLabel.Height = 13
        EditLabel.Caption = 'Number of Processes (>=2)'
        TabOrder = 0
        Text = '9'
        Value = 9
      end
      object Button1: TButton
        Left = 184
        Top = 32
        Width = 89
        Height = 49
        Caption = 'Run'
        TabOrder = 1
      end
    end
  end
end
