
const mongoose = require('mongoose');
const trips = require('./trips.json');

// Register the trip model.
require('../app_server/models/trips');

const Trip = mongoose.model('trips');

async function seedDatabase() {
    try {
        await mongoose.connect('mongodb://127.0.0.1/travlr');

        console.log('Connected to MongoDB');

        for (const trip of trips) {
            await Trip.updateOne(
                { code: trip.code },
                { $set: trip },
                { upsert: true, runValidators: true }
            );
        }

        console.log(`Successfully seeded ${trips.length} trips`);
    } catch (error) {
        console.error('Database seeding failed:', error);
        process.exitCode = 1;
    } finally {
        await mongoose.disconnect();
    }
}

seedDatabase();
