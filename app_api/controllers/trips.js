const mongoose = require('mongoose');
const Trip = mongoose.model('trips');

// GET: /trips - lists all the trips
// Regardless of outcome, response must include HTML status code
// and JSON message to the requesting client
const tripsList = async (req, res) => {
    try {
        const trips = await Trip.find({});

        if (!trips) {
            return res
                .status(404)
                .json({"message": "trips not found"});
        } else {
            return res
                .status(200)
                .json(trips);
        }
    } catch (err) {
        return res
            .status(404)
            .json(err);
    }
};
        
// GET: /trips/:tripCode - returns a single trip
const tripsFindByCode = async (req, res) => {
    const q = await Trip
        .find({ 'code': req.params.tripCode });

    if (!q) {
        return res
            .status(404)
            .json({"message": "trip not found"});
    } else {
        return res
            .status(200)
            .json(q);
    }
};

// POST: /trips - add a new trip
const tripsAddTrip = async (req, res) => {
    try {
        const trip = await Trip.create(req.body);
        return res.status(201).json(trip);
    } catch (err) {
        return res.status(400).json(err);
    }
};

// PUT: /trips/:tripCode - update an existing trip
const tripsUpdateTrip = async (req, res) => {
    try {
        const trip = await Trip.findOneAndUpdate(
            { code: req.params.tripCode },
            req.body,
            { new: true, runValidators: true }
        );

        if (!trip) {
            return res.status(404).json({ message: "trip not found" });
        }

        return res.status(200).json(trip);
    } catch (err) {
        return res.status(400).json(err);
    }
};

// DELETE: /trips/:tripCode - delete an existing trip
const tripsDeleteTrip = async (req, res) => {
    try {
        const trip = await Trip.findOneAndDelete({
            code: req.params.tripCode
        });

        if (!trip) {
            return res.status(404).json({ message: "trip not found" });
        }

        return res.status(200).json({
            message: "trip deleted successfully"
        });
    } catch (err) {
        return res.status(400).json(err);
    }
};



module.exports = {
    tripsList,
    tripsFindByCode,
    tripsAddTrip,
    tripsUpdateTrip,
    tripsDeleteTrip
};
