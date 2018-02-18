from datetime import timedelta, date

def daterange(start_date, end_date):
    for n in range(int ((end_date - start_date).days)):
        yield start_date + timedelta(n)

start_date = date(2017, 9, 30)
end_date = date(2017, 10, 23)
for single_date in daterange(start_date, end_date):
    print './neg_runner.sh', single_date.strftime("%Y-%m-%d")
    print './pos_runner.sh', single_date.strftime("%Y-%m-%d")
