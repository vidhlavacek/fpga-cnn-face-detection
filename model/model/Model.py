#Neural Network model

#Architecture

#Imports
import torch
import torch.nn as nn


#Convolution block
#CONVOLUTION -> Batch Norm -> ReLU
class ConvBNReLU(nn.Module):
    def __init__(self, in_channels, out_channels, kernel_size=3, stride=1):
        super().__init__()

        padding = kernel_size // 2

        self.layers = nn.Sequential(
            nn.Conv2d(
                in_channels=in_channels,
                out_channels=out_channels,
                kernel_size=kernel_size,
                stride=stride,
                padding=padding,
                bias=False
            ),
            nn.BatchNorm2d(out_channels),
            nn.ReLU(inplace=True)
        )

    def forward(self, x):
        return self.layers(x)

#CNN - FCOS
class MiniFCOSFaceV1(nn.Module):
    """
    Input:  [B, 3, 320, 320]
    Output: [B, 6, 40, 40]

    Output channels:
      0 -> face_logit
      1 -> left
      2 -> top
      3 -> right
      4 -> bottom
      5 -> centerness_logit
    """

    def __init__(self):
        super().__init__()

        self.stem = ConvBNReLU(3, 16, kernel_size=3, stride=2)
        self.block1 = ConvBNReLU(16, 24, kernel_size=3, stride=2)
        self.block2 = ConvBNReLU(24, 48, kernel_size=3, stride=2)

        self.head = nn.Sequential(
            ConvBNReLU(48, 48, kernel_size=3, stride=1),
            nn.Conv2d(
                in_channels=48,
                out_channels=6,
                kernel_size=1,
                stride=1,
                padding=0
            )
        )
    def forward(self, x):
        x = self.stem(x)      
        x = self.block1(x)    
        x = self.block2(x)    
        output = self.head(x) 
        return output