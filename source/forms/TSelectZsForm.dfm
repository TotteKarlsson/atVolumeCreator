object SelectZsForm: TSelectZsForm
  Left = 0
  Top = 0
  Caption = 'Select Z'#39's'
  ClientHeight = 328
  ClientWidth = 538
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 538
    Height = 265
    Align = alClient
    TabOrder = 0
    ExplicitLeft = 168
    ExplicitTop = 120
    ExplicitWidth = 553
    ExplicitHeight = 257
    object Splitter1: TSplitter
      Left = 209
      Top = 1
      Width = 6
      Height = 263
      ExplicitLeft = 243
      ExplicitHeight = 424
    end
    object Panel3: TPanel
      Left = 215
      Top = 1
      Width = 88
      Height = 263
      Align = alLeft
      TabOrder = 0
      ExplicitLeft = 159
      ExplicitTop = -4
      DesignSize = (
        88
        263)
      object MoveToUnselectLBBtn: TButton
        Left = 6
        Top = 92
        Width = 75
        Height = 50
        Anchors = [akTop, akRight]
        Caption = '->'
        Enabled = False
        TabOrder = 0
        OnClick = MoveItems
      end
      object MoveToSelectedLBBtn: TButton
        Left = 6
        Top = 148
        Width = 75
        Height = 50
        Anchors = [akTop, akRight]
        Caption = '<-'
        Enabled = False
        TabOrder = 1
        OnClick = MoveItems
      end
    end
    object GroupBox1: TGroupBox
      Left = 1
      Top = 1
      Width = 208
      Height = 263
      Align = alLeft
      Caption = 'Selected'
      TabOrder = 1
      object SelectedItems: TListBox
        Left = 2
        Top = 15
        Width = 204
        Height = 246
        Align = alClient
        ItemHeight = 13
        MultiSelect = True
        TabOrder = 0
        OnClick = ItemsClick
        ExplicitLeft = 1
        ExplicitTop = 1
        ExplicitWidth = 160
        ExplicitHeight = 309
      end
    end
    object GroupBox2: TGroupBox
      Left = 303
      Top = 1
      Width = 234
      Height = 263
      Align = alClient
      Caption = 'Unselected'
      TabOrder = 2
      ExplicitLeft = 286
      ExplicitTop = 80
      ExplicitWidth = 185
      ExplicitHeight = 105
      object UnSelectedItems: TListBox
        Left = 2
        Top = 15
        Width = 230
        Height = 246
        Align = alClient
        ItemHeight = 13
        MultiSelect = True
        TabOrder = 0
        OnClick = ItemsClick
        ExplicitLeft = 465
        ExplicitTop = 1
        ExplicitWidth = 135
        ExplicitHeight = 309
      end
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 265
    Width = 538
    Height = 63
    Align = alBottom
    TabOrder = 1
    ExplicitTop = 504
    ExplicitWidth = 825
    DesignSize = (
      538
      63)
    object Button2: TButton
      Left = 453
      Top = 6
      Width = 75
      Height = 50
      Anchors = [akTop, akRight]
      Caption = 'OK'
      ModalResult = 1
      TabOrder = 0
      ExplicitLeft = 331
    end
    object Button3: TButton
      Left = 357
      Top = 6
      Width = 75
      Height = 50
      Anchors = [akTop, akRight]
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 1
      ExplicitLeft = 235
    end
  end
end
