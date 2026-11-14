

const apiOptions = {
    server: 'http://localhost:3000'
};

/* GET travel view */
const travlr = (req, res) => {
    const path = '/api/trips';
    const requestOptions = {
    url: `${apiOptions.server}${path}`
};

 fetch(requestOptions.url)
    .then(response => response.json())
   .then(body => {
    let message = null;
    if (!(body instanceof Array)) {
        message = "API lookup error";
        body = [];
    } else {
        if (!body.length) {
            message = "No trips exist in our database!";
        }
    }

    res.render('travel', {
        title: 'Travlr Getaways',
        trips: body,
        message
    });
})
    .catch(err => {
        console.error(err);
    });
};

module.exports = {
    travlr
};
