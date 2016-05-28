<h1>Vending Machine</h1>
<h2>Overview</h2>

This example realizes a simple vending machine. The envisioned machine looks like this ![image in the doc folder](doc/vending.png).

To compile the example just call make. It is assumed that the code generator from sinelabore is installed (location see Makefile).

After startup you can select a product "A", "B" or "C" by pressing one of theses buttons on your keyboard.

Then you have to insert the coins to pay for the product. Only 10Ct, 20Ct and 50Ct coins are supported. To "insert" them click "1","2" or "5" on your keyboard.

After you paid the product will be released and you receive back you change.

<h2>Implementation</h2>

Most of the code is automatically generated from the UML file. It contains two state machines and an activity diagram. The state machines receive events from a message queue (see lib folder) and can also send messages to other state machines. This is a well known design pattern to decouple objects. A timer service (see lib folder) provides basic timer services that can be used from the state machines to realize delays or repetitive activities.


<h3> Product Store State Machine</h3>

The "product_store_sm" implements a simple product store containing the goods. It is responsible to release the products by turning on the motor etc.

<h3> Vending Machine</h3>
This is the controller machine keeping things together.

<h3> Change Algorithm</h3>
This is a simple algorithm modeled with the help of an activity diagram to release the right amount of money if the user overpaid. It takes care if one coin type is empty.
