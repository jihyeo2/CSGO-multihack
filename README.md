# Jerry's fart stinks

This is my very first CSGO multihack after messing around CSGO and other FPS games and writing mini cheats for a couple of years. Why the name? I don't remember anything but the fact that it was around 3 or 4am when I came up with this name.

---

## Important Notice

I didn't do any work around bypassing anticheat, so this only works on private servers.

---

## Preview

<<< video >>>

---

## Features

⚔️ **Combat Assist**

&nbsp;&nbsp;&nbsp;&nbsp;🧠 Aimbot [F1] – Automatically locks aim onto enemies for precise targeting  
&nbsp;&nbsp;&nbsp;&nbsp;🔫 Triggerbot [F2] – Automatically fires when an enemy enters your crosshair  
&nbsp;&nbsp;&nbsp;&nbsp;🎯 Recoil Control [F3] – Removes weapon recoil for steady aim  


🧱 **ESP (Extra Sensory Perception) Features**

&nbsp;&nbsp;&nbsp;&nbsp;🗺️ Radar Hack [F4] – Displays enemy positions on the in-game radar  
&nbsp;&nbsp;&nbsp;&nbsp;✨ Glow Outline [F5] – Adds glowing outlines:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- 🔵 Allies outlined in blue  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- 🟢 Enemies outlined in green  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- 🩸 Health-Based Model Color – Gradually tints entity textures red as their health drops  
&nbsp;&nbsp;&nbsp;&nbsp;👥 Show Teammates [F6] – Includes allies in all ESP visualizations  
&nbsp;&nbsp;&nbsp;&nbsp;📍 Snaplines [F7] – Draws a line from your position to each visible entity  
&nbsp;&nbsp;&nbsp;&nbsp;🟦 2D Box ESP [F8] – Draws a 2D box around each visible entity  
&nbsp;&nbsp;&nbsp;&nbsp;📊 Status Bars [F9] – Displays health and armor bars next to entities  
&nbsp;&nbsp;&nbsp;&nbsp;🧾 Status Text [F11] – Shows numeric health and armor values  
&nbsp;&nbsp;&nbsp;&nbsp;🧊 3D Box ESP [Home] – Renders a 3D bounding box around entities  
&nbsp;&nbsp;&nbsp;&nbsp;🧭 Velocity Indicator [Delete] – Draws a line showing the entity's movement direction  
&nbsp;&nbsp;&nbsp;&nbsp;👁️ View Angle Indicator [PgUp] – Draws a line showing where the entity is looking

➕ **Visual Assist**

&nbsp;&nbsp;&nbsp;&nbsp;🎯 Recoil Crosshair [PgDn] – Adds a static crosshair showing where bullets would land without recoil

---

## Installation Guide

0. Temporarily disable your antivirus (yep, not ideal...no stealth yet)
1. Download the latest release (`csgo_multihack_01.dll` and `DLLInjector_csgo.multihack.exe`) from this repo
2. Launch CSGO
3. Run the executable 

**Just a heads-up: I just realized most browsers will flag the executable as a virus when you try to download it. So you’ll probably need to disable your browser’s antivirus as well temporarily—at least until Julie Oh figures out a clever workaround. haha...haha...ha...tears**  

Double-check you see `csgo_multihack_01.dll` in the same folder the executable `DLLInjector_csgo.multihack.exe` is saved. Also, make sure CSGO is already running before you launch the cheat. 

---

## For devs (or anyone curious)

I wrote posts for a few of the parts I found interesting to talk about.

- [Aimbot - Not about calcAngle...then what? Hint: Ray](/16th/posts/aimbot)
- [ESP - I made my 3D box spin...pretty cool huh. And I can't make it stop, I gotta fix my code)](/16th/posts/esp)
- [GlowObjectManager - Took me 5 days to figure it out. If you can make it faster, I will...](/16th/posts/glowobjectmanager)

---

## Future Improvements/Goals

- Triggerbot needs serious fixing
- Glow (not the outline) doesn't get cleared when you turn off the glowhack
- Certain Combat Assists (Triggerbot & Recoil control) don't work well when used with other combat assists
- Recoil control could use more diverse per-weapon settings