
// File copyright (c) Rob Nelson, 2021.  All Rights Reserved. 
// Please feel free to use this code for your own purposes using Sagebox.  Sagebox is free for personal use, but donations are appreciated.
// www.projectsagebox.com -- e-mail me at rob@projectsagebox.com

// ***********************************************************************************
// -------------------
// Binary Tree Example
// -------------------
//
// This is an example of printing out a binary tree.  It's formed around a specific
// example of a 4-level binary tree with exactly 12 values, for a Quora question.
//
// I will be building larger examples and some generic functions for this in the future.
//
// This shows how to build the binary tree as well as using graphics function and
// the helper classes in Sagebox to draw the tree. 
//
// **********************************************************************************

#include "CSageBox.h"

// Use Globals in lieu of Class Structure, for simplicity

static constexpr int iRadius                = 12;
static constexpr int iMaxDepth              = 4;                    // Max Tree Depth
static constexpr int iHeight                = 60;                   // Height between nodes
static constexpr int iDist                  = 10*2;                 // Distance between left/right circles
static constexpr RgbColor rgbLineColor      = { 255,255,255 };      // Color for lines and circles
static constexpr RgbColor rgbCircleColor    = { 255,0,0 };          // Color for lines and circles
static constexpr const char * sTextColor    = "white";              // Color for node value output
static constexpr const char * sTextFont     = "Arial,15";           // Font for node value output

// Basic node structure, with a function to add sample integer node data. 
//
struct stNode_t
{
    int iData;
    stNode_t * pLeft;
    stNode_t * pRight;

    // Add a left and right node -- only add the node if the integer data > 0
    //
    void AddNode(int iLeft,int iRight)
    {
        if (iLeft > 0) *(pLeft = new stNode_t) = {iLeft};
        if (iRight > 0) *(pRight = new stNode_t) = {iRight};
    }
};

// PrintText() -- Print text centered in the circle
//
// This can be any format, and is just the one I chose for an example
//
void PrintText(CWindow & cWin,stNode_t * stNode,CfPoint & pLoc)
{
    if (stNode)
    {
        CString cs = CString() << stNode->iData; 
        CPoint szSize = cWin.GetTextSize(cWin.GetFont(sTextFont),cs);
        cWin.Write(pLoc - szSize/2,cs,fgColor(sTextColor) | Font(sTextFont));
    }
};

// DrawNode() -- Main Tree-Drawing Function
//
// Some of this is not need to get the idea of the algorithm. 
// The text does not need to be drawn.
//
// The circles are at the end points and don't specifically need to be drawn. 
// The circles could be text or anything and are just for show.
//
// Note that the circles and text are drawn after the next call to DrawNode()
// This is because the line will overlap the current endpoint, so the circles and text
// are drawn later to cover the lines drawn to the next set of nodes. 
//
void DrawNode(CWindow & cWin,CfPoint pPos,stNode_t * stNode)
{
    if (!stNode) return; 
    static int iDepth = 0;

    int iAddDist = (int) pow(2,(double) (iMaxDepth-iDepth))*iDist/2;
 
    iDepth++;

    CfPoint pLeft =  pPos + CPoint{-iAddDist, iHeight };
    CfPoint pRight = pPos + CPoint{ iAddDist, iHeight };
 
    if (stNode->pLeft)  cWin.DrawLine(pPos,pLeft,rgbLineColor);
    if (stNode->pRight) cWin.DrawLine(pPos,pRight,rgbLineColor);

    if (iDepth < iMaxDepth) DrawNode(cWin,pLeft,stNode->pLeft);
    if (iDepth < iMaxDepth) DrawNode(cWin,pRight,stNode->pRight);
    
    // Draw these later so they the drawn lines don't overlap.
    
    if (stNode->pLeft)  cWin.DrawCircle(pLeft,iRadius,rgbCircleColor);
    if (stNode->pRight) cWin.DrawCircle(pRight,iRadius,rgbCircleColor);
    PrintText(cWin,stNode->pLeft,pLeft);
    PrintText(cWin,stNode->pRight,pRight);

    iDepth--;
}

// DrawTree() -- Start the drawing by initiating the first node. 
//
void DrawTree(CWindow & cWin,int iDepth,stNode_t * stNode)
{
    // Get center if window, 10 pixels down 

    CfPoint pPos = CPoint{ cWin.GetWindowSize().cx/2, iRadius + 65 };

    DrawNode(cWin,pPos,stNode);

    // Draw the top circle and text 

    cWin.DrawCircle(pPos,iRadius,rgbCircleColor); 
    PrintText(cWin,stNode,pPos);
}

// Main Start
//
int main()
{
    // Create a window that also creates a Sagebox instance (AutoWindow()) since we only need the window.
    // 
    // bgGradient() -- Sets the background gradient and clears the window to this gradient
    // Title()      -- Sets the window title in the title bar
    //
    auto& cWin = CSagebox::AutoWindow(CSize(700,320),bgGradient("black","SkyBlueDark") | Title("Binary Tree Eample")); 

    // Add 12 pieces of data

    stNode_t stNode{1}; 
    stNode.AddNode(2,3); 
    stNode.pLeft->AddNode(4,5);
    stNode.pRight->AddNode(6,7);
    stNode.pRight->pRight->AddNode(8,-1);
    stNode.pLeft->pLeft->AddNode(9,10);
    stNode.pLeft->pRight->AddNode(11,12);

    // Add a label. 
    //
    // Font()           -- Sets the font for the label
    // JustCenterX()    -- Centers the widget in the X dimension
    // Transparent()    -- Blends the text to the background (since we have a gradient), rather than a solid background color.
    //
    cWin.TextWidget(0,15,"Binary Tree Example (12 Samples)",Font("Arial,25") | JustCenterX() | Transparent());

    // Draw the binary tree

    DrawTree(cWin,5,&stNode); 

    return cWin.WaitforClose(); // Wait for user to press the window close button.
}