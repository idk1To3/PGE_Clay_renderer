#define CLAY_IMPLEMENTATION
#include "clay.h"
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "PGEClayRenderer.h"

class PGEapplication : public olc::PixelGameEngine
{
public:
	PGEapplication()
	{
		sAppName = "Clay demo";
	}

private:
	PGEClayRenderer renderer;

public:
	bool OnUserCreate() override
	{  
		renderer.InitializeClay(this);

		treeImage = new ClayImage("assets\\tree.jpg");

		return true;
	}

private:
	int index = 0;

	ClayImage* treeImage;
	const Clay_String frenchFriesWiki = CLAY_STRING("        French fries, or simply fries, also known as chips, and finger chips (Indian English), are batonnet or julienne-cut deep-fried potatoes of disputed origin. They are prepared by cutting potatoes into even strips, drying them, and frying them, usually in a deep fryer. Pre-cut, blanched, and frozen russet potatoes are widely used, and sometimes baked in a regular or convection oven, such as an air fryer.\n\n        French fries are served hot, either soft or crispy, and are generally eaten as part of lunch or dinner or by themselves as a snack, and they commonly appear on the menus of diners, fast food restaurants, pubs, and bars. They are typically salted and may be served with ketchup, vinegar, mayonnaise, tomato sauce, or other sauces. Fries can be topped more heavily, as in the dishes of poutine, loaded fries or chili cheese fries, and are occasionally made from sweet potatoes instead of potatoes.\n\nPreparation\n\n        The standard method for cooking french fries is deep frying, which submerges them in a hot fat, typically oil.\n\n        The potatoes are prepared by first cutting them (peeled or unpeeled) into even strips, which are then wiped off or soaked in cold water to remove the surface starch, and thoroughly dried. They may then be fried in one or two stages. Chefs generally agree that the two-bath technique produces better results. Potatoes fresh out of the ground can have too high a water content resulting in soggy fries, so preference is for those that have been stored for a while.\n\n        In the two-stage or two-bath method, the first bath, sometimes called blanching, is in hot fat (around 160 °C/320 °F) to cook the fries through. This step can be done in advance. Then they are more briefly fried in very hot fat (190 °C/375 °F) to crisp the exterior. They are then placed in a colander or on a cloth to drain, then served. The exact times of the two baths depend on the size of the fries. For example, for 2–3 mm strips, the first bath takes about 3 minutes, and the second bath takes only seconds.\n\n        Since the 1960s, most french fries in the US have been produced from frozen Russet potatoes which have been blanched or at least air-dried industrially. The usual fat for making french fries is vegetable oil. In the past, beef suet was recommended as superior, with vegetable shortening as an alternative. McDonald's used a mixture of 93% beef tallow and 7% cottonseed oil until 1990, when they changed to vegetable oil with beef flavouring. Horse fat was standard in northern France and Belgium until recently, and is recommended by some chefs.\n\nChemical and physical changes\n\n        French fries are fried in a two-step process: the first time is to cook the starch throughout the entire cut at low heat, and the second time is to create the golden crispy exterior of the fry at a higher temperature. This is necessary because if the potato cuts are only fried once, the temperature would either be too hot, causing only the exterior to be cooked and not the inside, or not hot enough where the entire fry is cooked, but its crispy exterior will not develop. Although the potato cuts may be baked or steamed as a preparation method, this section will only focus on french fries made using frying oil. During the initial frying process (approximately 150 °C), water on the surface of the cuts evaporates off the surface and the water inside the cuts gets absorbed by the starch granules, causing them to swell and produce the fluffy interior of the fry.\n\n        The starch granules are able to retain the water and expand due to gelatinisation. The water and heat break the glycosidic linkages between amylopectin and amylose strands, allowing a new gel matrix to form via hydrogen bonds which aid in water retention. The moisture that gets trapped within the gel matrix is responsible for the fluffy interior of the fry. The gelatinised starch molecules move towards the surface of the fries ""forming a thick layer of gelatinised starch"" and this layer of pre-gelatinised starch becomes the crisp exterior after the potato cuts are fried for a second time. During the second frying process (approximately 180 °C), the remaining water on the surface of the cuts evaporates and the gelatinised starch molecules that collected towards the potato surface are cooked again, forming the crisp exterior. The golden-brown colour of the fry will develop when the amino acids and glucose on the exterior participate in a Maillard browning reaction.\n\nName and etymology\n\n        In the United States and most of Canada, the term french fries, sometimes capitalised as French fries, or shortened to fries, refers to all dishes of fried elongated pieces of potatoes. Variants in shape and size may have names such as curly fries, shoestring fries, etc.\n\n        In the United Kingdom, Australia, South Africa, Ireland and New Zealand, the term chips is generally used, being a popular dish in most Commonwealth countries. However, the term french fries or skinny fries is used for thinly cut fried potatoes which are different from the more traditional thick cut chips.\n\n        In the US or Canada these more thickly-cut chips might be called steak fries, depending on the shape, while the word chips is more often used in North America to refer to potato chips, commonly known in the UK, Ireland and South Africa as crisps. In Australia and New Zealand, chips are often referred to as hot chips to distinguish them from potato chips, although the type of 'chip' is often implied through context.\n\n        Thomas Jefferson had ""potatoes served in the French manner"" at a White House dinner in 1802. The expression ""french fried potatoes"" first occurred in print in English in the 1856 work Cookery for Maids of All Work by Eliza Warren: ""French Fried Potatoes. – Cut new potatoes in thin slices, put them in boiling fat, and a little salt; fry both sides of a light golden brown colour; drain."" This account referred to thin, shallow-fried slices of potato. It is not clear where or when the now familiar deep-fried batons or fingers of potato were first prepared. In the early 20th century, the term ""french fried"" was being used in the sense of ""deep-fried"" for foods like onion rings or chicken.\n\n        One story about the name ""french fries"" claims that when the American Expeditionary Forces arrived in Belgium during World War I, they assumed that chips were a French dish because French was spoken in the Belgian Army. But the name existed long before that in English, and the popularity of the term did not increase for decades after 1917. The term was in use in the United States as early as 1886. An 1899 item in Good Housekeeping specifically references Kitchen Economy in France: ""The perfection of French fries is due chiefly to the fact that plenty of fat is used.""\n");

	const Clay_Color ccPanel = Clay_Color(70, 70, 90, 255);
	const Clay_Color ccButtonUnselected = Clay_Color(125, 135, 152, 255);
	const Clay_Color ccButtonSelected = Clay_Color(162, 170, 185, 255);
	const Clay_Color ccWhite = Clay_Color(255, 255, 255, 255);

	void ClayButton(Clay_String buttonId, Clay_String buttonText, int buttonIndex)
	{
			CLAY
			({
				.id = CLAY_SID(buttonId),
				.layout =
				{
					.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIT()},
					.padding = {4,4,4,4},
					.childAlignment = CLAY_ALIGN_X_CENTER,
				},
				.backgroundColor = (index == buttonIndex ? ccButtonSelected : ccButtonUnselected),
				.cornerRadius = 5
			})
			{
				CLAY_TEXT(buttonText, CLAY_TEXT_CONFIG({.textColor = ccWhite, .fontSize = 16}));
			}
	}

	Clay_RenderCommandArray ComputeLayout()
	{
		Clay_BeginLayout();

		CLAY
		({
			.id = CLAY_ID("Layout"), //The background, which takes up the entire window
			.layout =
			{
				.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
				.padding = {10,10,10,10},
				.childGap = 10,
				.layoutDirection = CLAY_TOP_TO_BOTTOM
			},
			.backgroundColor = Clay_Color(20,20,25,255),
		})
		{
			CLAY
			({
				.id = CLAY_ID("Header"),
				.layout =
				{
					.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(80)},
					.padding = {20,20,20,20},
					.layoutDirection = CLAY_LEFT_TO_RIGHT,
				},
				.backgroundColor = ccPanel,
				.cornerRadius = 15,
				.clip = {.horizontal = true, .childOffset = Clay_GetScrollOffset()},
			})
			{ 
				CLAY_TEXT(CLAY_STRING("Clay (c layout) + PGE renderer demo"), CLAY_TEXT_CONFIG({.textColor = ccWhite, .fontSize = 32}));
			}

			CLAY
			({
				.id = CLAY_ID("Center panels"),
				.layout = 
				{
					.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
					.childGap = 10,
				},
			})
			{
				CLAY
				({
					.id = CLAY_ID("Left sidebar"),
					.layout = 
					{
						.sizing = {.width = CLAY_SIZING_FIT(100), .height = CLAY_SIZING_GROW()},
						.padding = {15,15,15,15},
						.childGap = 10,
						.layoutDirection = CLAY_TOP_TO_BOTTOM,
					},
					.backgroundColor = ccPanel,
					.cornerRadius = 15,
					.clip = {.vertical = true, .childOffset = Clay_GetScrollOffset()},
				})
				{ 
					ClayButton(CLAY_STRING("Unselection"), CLAY_STRING("None"), 0);
					ClayButton(CLAY_STRING("Text selection"), CLAY_STRING("Fries wikipedia"), 1);
					ClayButton(CLAY_STRING("Image selection"), CLAY_STRING("Tree image"), 2);
				}

				CLAY
				({
					.id = CLAY_ID("Content panel"),
					.layout =
					{
						.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
						.padding = {10,10,10,10},
						.childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER},
					},
					.backgroundColor = ccPanel,
					.cornerRadius = 15,
					})
				{

					if (index == 1)
					{
						CLAY
						({
							.id = CLAY_ID("Text"),
							.layout =
							{
								.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
								.padding = {20,20,20,20},
							},
							.cornerRadius = {15,15,15,15},
							.clip = {.vertical = true, .childOffset = Clay_GetScrollOffset() },
							.border = {.color = Clay_Color(0,140,160,255), .width = {5,5,5,5}},
							})
						{
							CLAY_TEXT(frenchFriesWiki, CLAY_TEXT_CONFIG({ .textColor = ccWhite, .fontSize = 16, .letterSpacing = 1, .lineHeight = 24 }));
						}
					}
					else if(index == 2)
					{
						CLAY
						({
							.id = CLAY_ID("Image"),
							.layout =
							{
								.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
							},
							.aspectRatio = treeImage->GetAspectRatio(),
							.image = {.imageData = treeImage},
							})
						{}
					}

				}
			}

		}

		return Clay_EndLayout();
	}

	bool inspecting = false;

public:
	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::RED);

		if (GetKey(olc::Key::I).bPressed && GetKey(olc::Key::CTRL).bHeld && GetKey(olc::Key::SHIFT).bHeld)
		{
			inspecting = !inspecting;
			Clay_SetDebugModeEnabled(inspecting);
		}

		renderer.UpdateInput(fElapsedTime);
		renderer.RenderLayout(ComputeLayout());

		Clay_ElementIdArray hovered = Clay_GetPointerOverIds();

		if (GetMouse(0).bPressed)
		{
			for (int i = hovered.length - 1; i >= 0; i--)
			{
				if (hovered.internalArray[i].id == CLAY_ID("Unselection").id)
					index = 0;

				if (hovered.internalArray[i].id == CLAY_ID("Text selection").id)
					index = 1;
				
				if (hovered.internalArray[i].id == CLAY_ID("Image selection").id)
					index = 2;
			}
		}

		return true;
	}

	bool OnUserDestroy() override
	{
		return true;
	}
};

int main()
{
	PGEapplication demo;
	if (demo.Construct(1200, 600, 1, 1, false, false, true, true))
		demo.Start();

	return 0;
}
