#include "ContactResolver.h"
#include "Console.h"

void ContactResolver::resolveContacts(ContactData *data)
{
	if (data->contacts->getLength() > 0)
	{
		// PREPARE CONTACTS
		for (int i = 0; i < data->contacts->getLength(); i++)
		{
			data->contacts->get(i)->calculateInternals();
		}

		//ADJUST POSITION
		int positionIterations = 2048;
		int positionIterationsUsed = 0;
		while (positionIterationsUsed < positionIterations)
		{
			// FIND BIGGEST PENETRATION
			float max = 0.01f;
			int index = -1;
			for (int i = 0; i < data->contacts->getLength(); i++)
			{
				if (data->contacts->get(i)->getPenetration() > max)
				{
					max = data->contacts->get(i)->getPenetration();
					index = i;
				}
			}
			if (index == -1)
				break;

			data->contacts->get(index)->applyPositionChange(max);

			/*for (int i = 0; i < data->contacts->getLength(); i++)
			{
				if (data->contacts->get(i)->getBody1() == data->contacts->get(index)->getBody1() ||
					data->contacts->get(i)->getBody1() == data->contacts->get(index)->getBody2() ||
					data->contacts->get(i)->getBody2() == data->contacts->get(index)->getBody1() ||
					data->contacts->get(i)->getBody2() == data->contacts->get(index)->getBody2())
				{
					data->contacts->get(i)->updatePenetration();
				}
			}*/

			positionIterationsUsed++;
		}

		//ADJUST VELOCITY
		int velocityIterations = 2048;
		int velocityIterationsUsed = 0;
		while (velocityIterationsUsed < velocityIterations)
		{
			// FIND BIGGEST DESIRED VELOCITY
			float max = 0.01f;
			int index = -1;
			for (int i = 0; i < data->contacts->getLength(); i++)
			{
				if (data->contacts->get(i)->getDesiredVelocity() > max)
				{
					max = data->contacts->get(i)->getDesiredVelocity();
					index = i;
				}
			}
			if (index == -1)
				break;

			data->contacts->get(index)->applyVelocityChange(max);

			/*for (int i = 0; i < data->contacts->getLength(); i++)
			{
				if (data->contacts->get(i)->getBody1() == data->contacts->get(index)->getBody1() ||
					data->contacts->get(i)->getBody1() == data->contacts->get(index)->getBody2() ||
					data->contacts->get(i)->getBody2() == data->contacts->get(index)->getBody1() ||
					data->contacts->get(i)->getBody2() == data->contacts->get(index)->getBody2())
				{
					data->contacts->get(i)->updateDesiredVelocity();
				}
			}*/

			velocityIterationsUsed++;
		}
	}

	delete data;

	// STEP 5 REPEAT
}