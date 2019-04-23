# CHESS GAME
Chess game build in Unreal Engine 4 (Version 4.22). This project in still under construction. 

There is a video which demonstrates a bit older version of the game.

[![GAME SCREEN SHOT](https://github.com/fastafaryan/chess/blob/master/screenshot.png)](https://youtu.be/p-h9v0Uo8i8)

# API

## ABOARD
Chess board.
### Functions
| Function Name  | Parameters | Return Type | Description |
| --- | --- |--- |--- |
| ABoard  | - | - | Constructor for class. Creates 64 grids as static meshes and attaches them to root component. |
| BeginPlay  | - | void | Defines default materials for grids, such as light or dark. |
| HighlightGrid  | UStaticMeshComponent* Grid | void | Changes grid's material to highlighted material.  |
### Properties
| Property Name | Type | Description |
| --- | --- | --- |
| BoxComponent | UBoxComponent* | Used as root component. |
| StaticMeshes | TArray<class  UStaticMeshComponent*> | Pointer array to hold every grid's address. |
| LightMaterial | UMaterial | Light colored grid material for the white chess grids. |
| DarkMaterial | UMaterial | Dark colored grid material for the black chess grids. |
| HighlightedMaterial | UMaterial | Grid's material is changed to this when player has ability to move to this grid. |
| SelectedMaterial | UMaterial | Grid's material is changed to this when player has selected to move to this grid. |
| DefaultMaterial | UMaterial | This is null by default. Only used as temporary pointer when grid's original material is changed to highlighted or selected material. |

## APARENTCLASS
This class is a parent class for all chess pieces.
### Functions
| Function Name  | Parameters | Return Type | Description |
| --- | --- |--- |--- |
| AParentClass | - | - | Constructor for class. Gets materials then assigns them to variables. |
| SetSelected | - | void | Call when actor is selected by mouse click. Changes actor's material to selected material to indicate it is selected.|
| SetDeselected | - | void | Called when actor is deselected. Changes actor's material to it's original material.|
| SetEliminateMaterial | - | void | Changes pieces material to eliminate material if this piece can be eliminated by making move. | 
| ChangeLocation | FVector MoveLocation | void | Called when actor is moved by the player. Gets one parameter which holds location to move. | 
| DetectSelectableGrids | SelectableGrids, DefaultMaterials | void | Detects selectable grids to move the piece. Gets 2 parameters. After function complete, first one holds selectable grid's addresses second one holds those grid's default materials sequentially. | 
| Eliminate | - | void | Eliminates piece from game. |
### Properties
| Property Name | Type | Description |
| --- | --- | --- |
| WhiteMaterial | UMaterial | White material for the piece. |
| BlackMaterial | UMaterial | Black material for the piece. |
| SelectedMaterial | UMaterial | Material to set when piece is selected. |
| EliminateMaterial | UMaterial | Material to set when piece could be eliminated. |

## APAWNACTOR
Pawn piece class. This class is a child of AParentActor. DetectSelectableGrids is overridden as pawn can make moves.
### Functions
| Function Name  | Parameters | Return Type | Description |
| --- | --- |--- |--- |
| APawnActor | - | - | Constructor for class. Loads static mesh for pawn. |
| DetectSelectableGrids | SelectableGrids, DefaultMaterials | void | Overidden function from AParentClass. It calculates pawn's possible next movements. |
### Properties
Has the same properties as AParentActor has.

## AROOKACTOR
Rook piece class. This class is a child of AParentActor. DetectSelectableGrids is overridden as rook can make moves.
### Functions
| Function Name  | Parameters | Return Type | Description |
| --- | --- |--- |--- |
| ARookActor | - | - | Constructor for class. Loads static mesh for rook. |
| DetectSelectableGrids | SelectableGrids, DefaultGrids | void | Overidden function from AParentClass. It calculates rook's possible next movements. |
| AddAndHighlight | SelectableGrids, DefaultMaterials, OverlappedActors, OverlappedComponents | void | Highlights every selectable grids. Also adds grid to SelectableGrids array and their default material to DefaultMaterials array. | 
### Properties
Has the same properties as AParentActor has.

## ABISHOPACTOR
Bishop piece class. This class is a child of AParentActor. DetectSelectableGrids is overridden as bishop can make moves.
### Functions
| Function Name  | Parameters | Return Type | Description |
| --- | --- |--- |--- |
| ABishopActor | - | - | Constructor for class. Loads static mesh for bishop. |
| DetectSelectableGrids | SelectableGrids, DefaultGrids | void | Overidden function from AParentClass. It calculates bishop's possible next movements. |
| AddAndHighlight | SelectableGrids, DefaultMaterials, OverlappedActors, OverlappedComponents | void | Highlights every selectable grids. Also adds grid to SelectableGrids array and their default material to DefaultMaterials array. | 
### Properties
Has the same properties as AParentActor has.

## AKNIGHTACTOR
Knight piece class. This class is a child of AParentActor. DetectSelectableGrids is overridden as knight can make moves.
### Functions
| Function Name  | Parameters | Return Type | Description |
| --- | --- |--- |--- |
| AKnightActor | - | - | Constructor for class. Loads static mesh for knight. |
| DetectSelectableGrids | SelectableGrids, DefaultGrids | void | Overidden function from AParentClass. It calculates knight's possible next movements. |
| CheckOverlap | SpawnLocation, SelectableGrids, DefaultMaterials | void | Checks overlap for possibly moveable locations. If overlap happens calls AddAndHighlight function. |
| AddAndHighlight | SelectableGrids, DefaultMaterials, OverlappedActors, OverlappedComponents | void | Highlights every selectable grids. Also adds grid to SelectableGrids array and their default material to DefaultMaterials array. | 
### Properties
Has the same properties as AParentActor has.

## AQUEENACTOR
Queen piece class. This class is a child of AParentActor. DetectSelectableGrids is overridden as queen can make moves.
### Functions
| Function Name  | Parameters | Return Type | Description |
| --- | --- |--- |--- |
| AQueenActor | - | - | Constructor for class. Loads static mesh for queen. |
| DetectSelectableGrids | SelectableGrids, DefaultGrids | void | Overidden function from AParentClass. It calculates queen's possible next movements. |
| CheckOverlap | SpawnLocation, SelectableGrids, DefaultMaterials | void | Checks overlap for possibly moveable locations. If overlap happens calls AddAndHighlight function. |
| AddAndHighlight | SelectableGrids, DefaultMaterials, OverlappedActors, OverlappedComponents | void | Highlights every selectable grids. Also adds grid to SelectableGrids array and their default material to DefaultMaterials array. | 
### Properties
Has the same properties as AParentActor has.

## AKINGACTOR
King piece class. This class is a child of AParentActor. DetectSelectableGrids is overridden as king can make moves.
### Functions
| Function Name  | Parameters | Return Type | Description |
| --- | --- |--- |--- |
| AKingActor | - | - | Constructor for class. Loads static mesh for king. |
| DetectSelectableGrids | SelectableGrids, DefaultGrids | void | Overidden function from AParentClass. It calculates king's possible next movements. |
| CheckOverlap | SpawnLocation, SelectableGrids, DefaultMaterials | void | Checks overlap for possibly moveable locations. If overlap happens calls AddAndHighlight function. |
| AddAndHighlight | SelectableGrids, DefaultMaterials, OverlappedActors, OverlappedComponents | void | Highlights every selectable grids. Also adds grid to SelectableGrids array and their default material to DefaultMaterials array. | 
### Properties
Has the same properties as AParentActor has.

## Beacuse of the reasons I am to lazy I haven't written API for the all classes. However hopefully it will come back soon...
